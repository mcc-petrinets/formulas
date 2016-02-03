#!/usr/bin/env python

'''
smc [OPTIONS] PNML FORMULA

The Simple Model Checker is a bounded CTL model checker

PNML is the path to a PNML file
FORMULA is the path to an XML file containing formulas (MCC 2015 format)

And OPTIONS is zero or more of the following options

 --max-depth=N
   Maximum length of a firing sequence that will be explored when
   constructing the bounded fragment of the state space that the model
   checker uses to evaluate each formula.

 --max-states=N
   Limit on the number of markings that the initial fragment of the
   reachability graph considered by this tool will have.

 --mcc15-stop-after=N
   Stop verifying formulas from the formula file FORMULA after having found
   N formulas for which the tool was unable to decide

 --explore-only
   Ignore the FORMULA file (you will still have to provide an argument) and
   rather use the formula "false".

 --use10
   Try to solve formulas using a bounded state space with only 10 states, prior
   to solving them using the regular bounded state space.

 --help
   Prints this message
'''

try :
    import os
    import sys
    import time
    import resource
    import argparse
    import networkx
    import xml.etree.ElementTree
except Exception, e:
    print 'ERROR!'
    print 'It seems that your python installation is missing some package.'
    print 'This tool requires, among others, argparse, and networkx'
    print 'The runtime reported the following error:\n\n', str (e)
    print 'You might want to use "easy_install --user PACKAGE"'
    sys.exit (1)
finally :
    if sys.version_info < (2, 7, 0) or sys.version_info >= (3, 0, 0) :
        print ("")
        print ("*** ERROR ***")
        print ("This tool relies on Python 2.7!!")
        print ("Install Python 2.7 or modify the first line of the file 'smc.py' so that it calls Python 2.7")
        print ("")
        sys.exit (1)

import ptnet

class BoundedSearch :

    # outcome of the CTL verification algorithm
    RESULT_SAT   = 'SAT'
    RESULT_UNSAT = 'UNSAT'
    RESULT_UNDEF = '?'

    def __init__ (self, net) :
        self.net = net
        self.max_depth = 3
        self.max_states = 9000
        self.states = networkx.DiGraph ()
        self.init_state = net.m0.clone ()
        self.__new_mark = 0
        self.__state_space_built = False

        self.stats_nr_states = 0
        self.stats_nr_states_fe = 0
        self.stats_nr_edges = 0
        self.stats_depth = 0
        self.stats_stop_reason = "'full state space'"

    def new_mark (self) :
        self.__new_mark += 1
        return self.__new_mark

    def find_and_insert (self, marking) :
        try :
            return self.states.node[marking]['org']
        except KeyError :
            self.states.add_node (marking, org=marking)
            return marking

    def build_state_space (self) :

        if self.__state_space_built : return

        # enable structural marking equality, rather than just checking the pointer id()
        ptnet.Marking.frozen_eq = False

        assert (self.init_state == self.net.m0)
        curr_depth = 0
        self.find_and_insert (self.init_state)
        layer = list ([self.init_state])
        next_layer = list ()

        # bread-first search on the state space
        print "smc: -----"
        print "smc: constructing state space"
        print "smc: limits: maximum depth:", self.max_depth
        print "smc: limits: maximum nr. of states:", self.max_states
        while (len (self.states) < self.max_states
                and curr_depth < self.max_depth
                and len (layer) >= 1) :
            print 'smc: build: so far', len(self.states), 'markings found'
            print 'smc: build: expanding', len (layer), 'markings at depth', curr_depth
            for marking in layer :
                #print 'smc: build: expanding marking', repr (marking)
                if (marking.fully_expanded) : continue
                make_fully_expanded = True
                for t in self.net.enabled (marking) :
                    #print "smc: firing transition %s" % str (t)
                    new_marking = self.net.fire (marking, t)
                    new_canon_marking = self.find_and_insert (new_marking)
                    #print 'smc: build: - adding edge to', repr (new_canon_marking), 'trans', repr (t)
                    self.states.add_edge (marking, new_canon_marking, trans=t)
                    if not new_canon_marking.fully_expanded : next_layer.append (new_canon_marking)
                    if len (self.states) >= self.max_states :
                        make_fully_expanded = False
                        break
                if make_fully_expanded :
                    marking.fully_expanded = True
                    self.stats_nr_states_fe += 1
                else :
                    curr_depth -= 1 # avoids bad reporting of reason 
                    break
            curr_depth += 1
            layer = next_layer
            next_layer = list ()

        self.__state_space_built = True
        self.stats_nr_states = self.states.number_of_nodes ()
        self.stats_nr_edges = self.states.number_of_edges ()
        self.stats_depth = curr_depth
        if curr_depth >= self.max_depth :
            self.stats_stop_reason = "'exceeded depth'"
        if len (self.states) >= self.max_states :
            self.stats_stop_reason = "'exceeded number of states'"

        print "smc: build: done,", \
            self.stats_nr_states, "states,", \
            self.stats_nr_states - self.stats_nr_states_fe, "not fully expanded,", \
            self.stats_nr_edges, "edges,", \
            self.stats_depth, "complete steps deep"
        print "smc: state-space construction terminated due to", self.stats_stop_reason

        # disable structural equality checking
        ptnet.Marking.frozen_eq = True

        #for s in self.states :
        #    assert (s.fully_expanded or len (self.states.successors (s)) == 0)

    def debug_print_states (self, formulas = None) :
        for s in self.states :
            self.debug_print_state (s, formulas)

        print "\ninitial state:"
        self.debug_print_state (self.init_state, formulas)

    def debug_print_state (self, s, formulas = None) :
        print "%r, %d successors%s" % (s, len (self.states.successors (s)), \
                "" if s.fully_expanded else ", not fully expanded")
        for ss in self.states.successors (s) :
            print "  edge  %-10r %r" % (self.states[s][ss]['trans'], ss)
        if formulas :
            print "  sat  ", [str (f) for f in formulas if s.is_sat (f)]
            print "  unsat", [str (f) for f in formulas if s.is_unsat (f)]
            print "  undef", [str (f) for f in formulas if s.is_undef (f)]
        else :
            print "  SAT  ", [str (f) for f in s.formulas_sat]
            print "  UNDEF", [str (f) for f in s.formulas_undef]

    def debug_print_states_sat (self, formula) :
        for s in self.states :
            if s.is_sat (formula) :
                self.debug_print_state (s, [formula])

    def debug_print_states_undef (self, formula) :
        for s in self.states :
            if s.is_undef (formula) :
                self.debug_print_state (s, [formula])

    def clear_state_labelling (self) :
        for marking in self.states :
            marking.formulas_sat.clear ()
            marking.formulas_undef.clear ()
        
    def label_states (self, formula) :
        if formula.op == Formula.TRUE :
            self.__label_states_true (formula)
        elif formula.op == Formula.FALSE :
            self.__label_states_false (formula)
        elif formula.op == Formula.IS_FIREABLE :
            self.__label_states_isfireable (formula)
        elif formula.op == Formula.LEQ :
            self.__label_states_leq (formula)
        elif formula.op == Formula.NOT :
            self.label_states (formula.sub1)
            self.__label_states_not (formula)
        elif formula.op == Formula.OR :
            self.label_states (formula.sub1)
            self.label_states (formula.sub2)
            self.__label_states_or (formula)
        elif formula.op == Formula.AND :
            self.label_states (formula.sub1)
            self.label_states (formula.sub2)
            self.__label_states_and (formula)
        elif formula.op == Formula.EX :
            self.label_states (formula.sub1)
            self.__label_states_ex (formula)
        elif formula.op == Formula.EU :
            self.label_states (formula.sub1)
            self.label_states (formula.sub2)
            self.__label_states_eu (formula)
        elif formula.op == Formula.EG :
            self.label_states (formula.sub1)
            self.__label_states_eg (formula)
        else :
            assert (False)

    def __label_states_true (self, formula) :
        print "smc: labelling formula:", formula
        assert (formula.op == Formula.TRUE)
        for marking in self.states :
            marking.formulas_sat.add (formula)

    def __label_states_false (self, formula) :
        print "smc: labelling formula:", formula
        assert (formula.op == Formula.FALSE)

    def __label_states_isfireable (self, formula) :
        print "smc: labelling formula:", formula
        assert (formula.op == Formula.IS_FIREABLE)
        assert (formula.atom_identifiers != None)

        nr_sat = 0

        for marking in self.states :
            found = False
            for transition in formula.atom_identifiers :
                if self.net.enables (marking, transition) :
                    found = True
                    break
            if found :
                marking.formulas_sat.add (formula)
                nr_sat += 1

        print "smc:   %d states sat, %d unsat, 0 undef" % \
                (nr_sat, len (self.states) - nr_sat)

    def __label_states_leq (self, formula) :
        print "smc: labelling formula:", formula
        assert (formula.op == Formula.LEQ)
        assert (formula.sub1 != None)
        assert (formula.sub2 != None)

        nr_sat = 0

        for marking in self.states :
            a = self.__label_states_eval_int (marking, formula.sub1)
            b = self.__label_states_eval_int (marking, formula.sub2)
            if a <= b :
                marking.formulas_sat.add (formula)
                nr_sat += 1

        print "smc:   %d states sat, %d unsat, 0 undef" % \
                (nr_sat, len (self.states) - nr_sat)

    def __label_states_eval_int (self, marking, formula) :
        if formula.op == Formula.TOKEN_COUNT :
            assert (formula.atom_identifiers != None)
            result = 0
            for place in formula.atom_identifiers :
                result += marking[place]
            return result
        elif formula.op == Formula.INT_CONST :
            assert (formula.atom_int != None)
            return formula.atom_int
        else :
            assert (False)

    def __label_states_not (self, formula) :
        print "smc: labelling formula:", formula
        assert (formula.op == Formula.NOT)
        assert (formula.sub1 != None)

        nr_sat = 0
        nr_undef = 0

        for marking in self.states :
            # if the subformula is UNDEF, then the NOT is also UNDEF
            if formula.sub1 in marking.formulas_undef :
                marking.formulas_undef.add (formula)
                nr_undef += 1
                continue
            # otherwise we can decide the NOT
            if formula.sub1 not in marking.formulas_sat :
                marking.formulas_sat.add (formula)
                nr_sat += 1

        print "smc:   %d states sat, %d unsat, %d undef; not" % \
                (nr_sat, len (self.states) - nr_sat - nr_undef, nr_undef)

    def __label_states_or (self, formula) :
        print "smc: labelling formula:", formula
        assert (formula.op == Formula.OR)
        assert (formula.sub1 != None)
        assert (formula.sub2 != None)

        nr_sat = 0
        nr_undef = 0

        for marking in self.states :
            # if at least one is SAT, then the OR is SAT
            if marking.is_sat (formula.sub1) or marking.is_sat (formula.sub2) :
                marking.formulas_sat.add (formula)
                nr_sat += 1
                continue

            # if both are UNSAT, then the OR is UNSAT
            if marking.is_unsat (formula.sub1) and marking.is_unsat (formula.sub2) :
                continue

            # otherwise the OR is UNDEF
            marking.formulas_undef.add (formula)
            nr_undef += 1

        print "smc:   %d states sat, %d unsat, %d undef; or" % \
                (nr_sat, len (self.states) - nr_sat - nr_undef, nr_undef)

    def __label_states_and (self, formula) :
        print "smc: labelling formula:", formula
        assert (formula.op == Formula.AND)
        assert (formula.sub1 != None)
        assert (formula.sub2 != None)

        nr_sat = 0
        nr_undef = 0

        for marking in self.states :
            # if both are SAT, then the AND is SAT
            if marking.is_sat (formula.sub1) and marking.is_sat (formula.sub2) :
                marking.formulas_sat.add (formula)
                nr_sat += 1
                continue

            # if one is UNSAT, then the AND is UNSAT
            if marking.is_unsat (formula.sub1) or marking.is_unsat (formula.sub2) :
                continue

            # otherwise the OR is UNDEF
            marking.formulas_undef.add (formula)
            nr_undef += 1

        print "smc:   %d states sat, %d unsat, %d undef; and" % \
                (nr_sat, len (self.states) - nr_sat - nr_undef, nr_undef)

    def __label_states_ex (self, formula) :
        print "smc: labelling formula:", formula
        assert (formula.op == Formula.EX)
        assert (formula.sub1 != None)

        nr_sat = 0
        nr_undef = 0

        for marking in self.states :
            # formula is SAT iff there is some successor where sub1 is SAT
            # formula is UNDEF iff it is not SAT and either marking is not
            #                       fully expanded or
            #                       there is some successor with sub1 UNDEF
            # formula is UNSAT in any other case
            found_sat = False
            found_undef = False
            for successor_marking in self.states.successors (marking) :
                if successor_marking.is_sat (formula.sub1) :
                    found_sat = True
                    break
                if successor_marking.is_undef (formula.sub1) :
                    found_undef = True

            if found_sat :
                marking.formulas_sat.add (formula)
                nr_sat += 1
                continue
            if not marking.fully_expanded or found_undef :
                marking.formulas_undef.add (formula)
                nr_undef += 1

        print "smc:   %d states sat, %d unsat, %d undef; EX" % \
                (nr_sat, len (self.states) - nr_sat - nr_undef, nr_undef)

    def __label_states_eg (self, formula) :
        print "smc: labelling formula:", formula
        assert (formula.op == Formula.EG)
        assert (formula.sub1 != None)

        # the algorithm here is more involved than in the previous cases,
        # we distinguish several cases:
        #
        # f = EG f1 at state s, with successors s' ...
        # 
        # f1 at s   successors                                  f at s
        # ========= =========================================== =======
        # SAT       s in a f1-SCC                               SAT
        #           not in f1-SCC but there is s' with f SAT    SAT
        #           for all s', f UNSAT and s fully exp.        UNSAT
        #           else                                        UNDEF
        # UNSAT                                                 UNSAT
        # UNDEF     for all s', f UNSAT and s fully exp.        UNSAT
        #           else                                        UNDEF
        # ========= =========================================== =======

        msat = self.new_mark ()
        munsat = self.new_mark ()

        self.__label_states_eg_mark_sat (formula.sub1, msat)
        self.__label_states_eg_mark_unsat (formula.sub1, munsat)

        nr_sat = 0
        nr_undef = 0

        for s in self.states :
            if s.m == msat :
                s.formulas_sat.add (formula)
                nr_sat += 1
            elif s.m != munsat :
                s.formulas_undef.add (formula)
                nr_undef += 1

        print "smc:   %d states sat, %d unsat, %d undef; EG" % \
                (nr_sat, len (self.states) - nr_sat - nr_undef, nr_undef)

    def __label_states_eg_mark_sat (self, f, msat) :
        work = []
        subg = self.states.subgraph (s for s in self.states if s.is_sat (f))
        sccs = networkx.algorithms.strongly_connected_components (subg)
        print "smc:   sat subgraph has %d states %d edges" % \
                (subg.number_of_nodes (), subg.number_of_edges ())
        for scc in [x for x in sccs if len (x) >= 2] :
            print "smc:  scc of size", len (scc)
            found = False
            for s in scc :
                if not s.is_sat (f) :
                    found = True
                    break
            if found :
                continue
            for s in scc :
                work.append (s)
                s.m = msat
        while len (work) :
            s = work.pop ()
            for ss in self.states.predecessors (s) :
                if ss.is_sat (f) and ss.m != msat :
                    work.append (ss)
                    ss.m = msat
        
    def __label_states_eg_mark_unsat (self, f, munsat) :
        work = []
        for s in self.states :
            if s.is_unsat (f) :
                work.append (s)
                s.m = munsat
        while len (work) :
            s = work.pop ()
            for ss in self.states.predecessors (s) :
                if ss.m != munsat and \
                        ss.fully_expanded and \
                        self.__are_all_succ_unsat (ss, munsat) :
                    work.append (ss)
                    ss.m = munsat

    def __label_states_eu (self, formula) :
        print "smc: labelling formula:", formula
        assert (formula.op == Formula.EU)
        assert (formula.sub1 != None)
        assert (formula.sub2 != None)

        # the algorithm here is more involved than in the previous cases,
        # we distinguish several cases:
        #
        # f = E (f1 U f2) at state s, with successors s' ...
        # 
        # f2 at s   f1 at s     successors                          f at s
        # ========= =========== =================================== =======
        # SAT                                                       SAT
        # UNSAT     SAT         exists s' with f SAT                SAT
        #                       in all s' f UNSAT and s fully exp.  UNSAT
        #                       else                                UNDEF
        #           UNSAT                                           UNSAT
        #           UNDEF       in all s' f UNSAT and s fully exp.  UNSAT
        #                       else                                UNDEF
        # UNDEF     SAT         exists s' with f SAT                SAT
        #                       else                                UNDEF
        #           else                                            UNDEF
        # ========= =========== =================================== =======

        msat = self.new_mark ()
        munsat = self.new_mark ()
        have_all_states = self.stats_nr_states_fe == self.stats_nr_states

        self.__label_states_eu_mark_sat (formula, msat)
        if not have_all_states : self.__label_states_eu_mark_unsat (formula, munsat)

        nr_sat = 0
        nr_undef = 0

        for s in self.states :
            if s.m == msat :
                s.formulas_sat.add (formula)
                nr_sat += 1
            elif not have_all_states and s.m != munsat :
                s.formulas_undef.add (formula)
                nr_undef += 1

        print "smc:   %d states sat, %d unsat, %d undef; EU" % \
                (nr_sat, len (self.states) - nr_sat - nr_undef, nr_undef)

    def __label_states_eu_mark_sat (self, f, msat) :
        work = []
        for s in self.states :
            if s.is_sat (f.sub2) :
                work.append (s)
                s.m = msat
        while len (work) :
            s = work.pop ()
            for ss in self.states.predecessors (s) :
                if ss.is_sat (f.sub1) and ss.m != msat :
                    work.append (ss)
                    ss.m = msat
        
    def __label_states_eu_mark_unsat (self, f, munsat) :
        work = []
        for s in self.states :
            if s.is_unsat (f.sub2) and s.is_unsat (f.sub1) :
                work.append (s)
                s.m = munsat
        while len (work) :
            s = work.pop ()
            for ss in self.states.predecessors (s) :
                if ss.m != munsat and \
                        ss.is_unsat (f.sub2) and \
                        ss.fully_expanded and \
                        self.__are_all_succ_unsat (ss, munsat) :
                    work.append (ss)
                    ss.m = munsat

    def __are_all_succ_unsat (self, s, munsat) :
        for ss in self.states.successors (s) :
            if ss.m != munsat : return False
        return True

    def verify (self, formula) :

        print "smc: original:"
        print "smc:", formula

        # rewrite the formula so it only uses the first 8 operators:
        # NOT, OR, EX, EU, EG, and atomic propositions (3)
        ident = formula.ident
        formula.rewrite ()
        print "smc: after rewriting:"
        print "smc:", formula

        # simplify the formula
        formula.simplify ()
        formula.ident = ident # could have been removed
        print "smc: after simplifying:"
        print "smc:", formula

        # construct a bounded fragment of the state space (if necessary)
        self.build_state_space ()

        # explore the syntax tree of the formula, starting from the
        # smallest subformulas and up to the larger ones, and labell every
        # state in self.states with the subformulas satisfied by that state
        print "smc: labelling states"
        self.label_states (formula)

        # at this point, we can decide satisfiability of the formula under
        # this fragment of the state space iff the formula labels the
        # initial state
        print "smc: labelling done!"
        if formula in self.init_state.formulas_undef :
            result = BoundedSearch.RESULT_UNDEF
        elif formula in self.init_state.formulas_sat :
            result = BoundedSearch.RESULT_SAT
        else :
            result = BoundedSearch.RESULT_UNSAT

        # clear the labelling of all states, this could be optimized, in
        # order to reuse it for the next formulas ...
        print "smc: clearing state labels"
        self.clear_state_labelling ()

        return result

class Formula :
    # types of nodes in the formula tree 
    TRUE         = 0
    FALSE        = 1
    IS_FIREABLE  = 2
    LEQ          = 3
    NOT          = 4
    OR           = 5
    AND          = 6
    EX           = 7
    EU           = 8
    EG           = 9
    # ..............
    TOKEN_COUNT  = 10
    INT_CONST    = 11
    A            = 12
    E            = 13
    G            = 14
    F            = 15
    X            = 16
    U            = 17

    def __init__ (self, op = None, sub1 = None, sub2 = None) :
        self.op = op
        self.sub1 = sub1
        self.sub2 = sub2
        self.atom_identifiers = None
        self.atom_int = None
        self.ident = None

    def rewrite (self) :
        # EF f          = E (True U f)
        # AX f          = not EX not f
        # AG f          = not E (True U not f)
        # AF f          = not EG not f
        # A (f U g)     = not E ((not g) U (not f and not g)) and (not EG not g)
        if self.op == Formula.E :
            if self.sub1.op == Formula.X :
                self.op = Formula.EX
                self.sub1 = self.sub1.sub1
                self.sub1.rewrite ()
            elif self.sub1.op == Formula.U :
                f = self.sub1.sub1
                g = self.sub1.sub2
                f.rewrite ()
                g.rewrite ()
                self.op = Formula.EU
                self.sub1 = f
                self.sub2 = g
            elif self.sub1.op == Formula.G :
                self.op = Formula.EG
                self.sub1 = self.sub1.sub1
                self.sub1.rewrite ()
            elif self.sub1.op == Formula.F :
                # EF f = E (True U f)
                f = self.sub1.sub1
                f.rewrite ()
                self.op = Formula.EU
                self.sub1 = Formula (Formula.TRUE)
                self.sub2 = f
            else :
                raise Exception, '"%s": cannot handle this subformula' % self
        elif self.op == Formula.A :
            if self.sub1.op == Formula.X :
                # AX f = not EX not f
                f = self.sub1.sub1
                f.rewrite ()
                not_f = Formula (Formula.NOT, f)
                self.op = Formula.NOT
                self.sub1 = Formula (Formula.EX, not_f)
            elif self.sub1.op == Formula.U :
                # A (f U g) = not E ((not g) U (not f and not g)) and (not EG not g)
                f = self.sub1.sub1
                g = self.sub1.sub2
                f.rewrite ()
                g.rewrite ()
                not_f = Formula (Formula.NOT, f)
                not_g = Formula (Formula.NOT, g)
                not_f_and_not_g = Formula (Formula.AND, not_f, not_g)
                not_eu = Formula (Formula.NOT, Formula (Formula.EU, not_g, not_f_and_not_g))
                not_eg_not_g = Formula (Formula.NOT, Formula (Formula.EG, not_g))
                self.op = Formula.AND
                self.sub1 = not_eu
                self.sub2 = not_eg_not_g
            elif self.sub1.op == Formula.G :
                # AG f = not E (True U not f)
                f = self.sub1.sub1
                f.rewrite ()
                not_f = Formula (Formula.NOT, f)
                self.op = Formula.NOT
                self.sub1 = Formula (Formula.EU, Formula (Formula.TRUE), not_f)
            elif self.sub1.op == Formula.F :
                # AF f = not EG not f
                f = self.sub1.sub1
                f.rewrite ()
                not_f = Formula (Formula.NOT, f)
                self.op = Formula.NOT
                self.sub1 = Formula (Formula.EG, not_f)
            else :
                raise Exception, '"%s": cannot handle this subformula' % self
        elif self.op == Formula.G or \
                self.op == Formula.F or \
                self.op == Formula.X or \
                self.op == Formula.U :
            raise Exception, '"%s": this seems to be an LTL formula that we cannot handle' % self
        elif self.op == Formula.TOKEN_COUNT or \
                self.op == Formula.INT_CONST :
            raise Exception, '"%s": this seems to be an integer expression that we cannot handle' % self
        elif self.op == Formula.NOT or \
                self.op == Formula.EX or \
                self.op == Formula.EG :
            self.sub1.rewrite ()
        elif self.op == Formula.OR or \
                self.op == Formula.AND or \
                self.op == Formula.EU :
            self.sub1.rewrite ()
            self.sub2.rewrite ()
        elif self.op == Formula.TRUE or \
                self.op == Formula.FALSE or \
                self.op == Formula.IS_FIREABLE or \
                self.op == Formula.LEQ :
            return
        else :
            raise Exception, '"%s": Formula::rewrite, internal error' % self

    def simplify (self) :
        if self.sub1 != None : self.sub1.simplify ()
        if self.sub2 != None : self.sub2.simplify ()

        # not (not f) = f
        if self.op == Formula.NOT and \
                self.sub1.op == Formula.NOT :
            self.__copy_fields (self.sub1.sub1)

        # not True = False
        elif self.op == Formula.NOT and \
                self.sub1.op == Formula.TRUE :
            self.op = Formula.FALSE

        # not False = True
        elif self.op == Formula.NOT and \
                self.sub1.op == Formula.FALSE :
            self.op = Formula.TRUE

        # f and f = f
        elif self.op == Formula.AND and \
                self.sub1.__easy_syntax_eq (self.sub2) :
            self.__copy_fields (self.sub1)

        # f and not f = False
        elif self.op == Formula.AND and \
                self.sub2.op == Formula.NOT and \
                self.sub1.__easy_syntax_eq (self.sub2.sub1) :
            self.op = Formula.FALSE

        # not f and f = False
        elif self.op == Formula.AND and \
                self.sub1.op == Formula.NOT and \
                self.sub2.__easy_syntax_eq (self.sub1.sub1) :
            self.op = Formula.FALSE

        # f or f = True
        elif self.op == Formula.OR and \
                self.sub1.__easy_syntax_eq (self.sub2) :
            self.op = Formula.TRUE

        # True or f = True
        elif self.op == Formula.OR and \
                self.sub1.op == Formula.TRUE :
            self.op = Formula.TRUE

        # f or True = True
        elif self.op == Formula.OR and \
                self.sub2.op == Formula.TRUE :
            self.op = Formula.TRUE

        # True and f = f
        elif self.op == Formula.AND and \
                self.sub1.op == Formula.TRUE :
            self.__copy_fields (self.sub2)

        # f and True = f
        elif self.op == Formula.AND and \
                self.sub2.op == Formula.TRUE :
            self.__copy_fields (self.sub1)

        # (not f) and (not g) = not (f or g)
        elif self.op == Formula.AND and \
                self.sub1.op == Formula.NOT and \
                self.sub2.op == Formula.NOT :
            self.op = Formula.NOT
            new_f = Formula (Formula.OR, self.sub1.sub1, self.sub2.sub1)
            self.sub1 = new_f

        # (not f) or (not g) = not (f and g)
        elif self.op == Formula.OR and \
                self.sub1.op == Formula.NOT and \
                self.sub2.op == Formula.NOT :
            self.op = Formula.NOT
            new_f = Formula (Formula.AND, self.sub1.sub1, self.sub2.sub1)
            self.sub1 = new_f

        # EX False = False
        # EG False = False
        # E (f U False) = False
        # EX True = True
        # EG True = True
        # E (f U True) = True
        elif (self.op == Formula.EX or self.op == Formula.EG) and \
                self.sub1.op == Formula.FALSE :
            self.op = Formula.FALSE
        elif self.op == Formula.EU and \
                self.sub2.op == Formula.FALSE :
            self.op = Formula.FALSE
        elif (self.op == Formula.EX or self.op == Formula.EG) and \
                self.sub1.op == Formula.TRUE :
            self.op = Formula.TRUE
        elif self.op == Formula.EU and \
                self.sub2.op == Formula.TRUE :
            self.op = Formula.TRUE

        # there are more ...
        # we could even identify common subformulas (!!)
        else :
            return

    def __copy_fields (self, g) :
        self.op = g.op
        self.sub1 = g.sub1
        self.sub2 = g.sub2
        self.atom_int = g.atom_int
        self.atom_identifiers = g.atom_identifiers
        self.ident = g.ident

    def __easy_syntax_eq (self, g) :
        if self.op != g.op : return False
        if self.op == Formula.IS_FIREABLE or self.op == Formula.TOKEN_COUNT :
            return set (self.atom_identifiers) == set (g.atom_identifiers)
        elif self.op == Formula.INT_CONST :
            return self.atom_int == g.atom_int
        elif self.op == Formula.TRUE or \
                self.op == Formula.FALSE :
                return True
        elif self.op == Formula.NOT or \
                self.op == Formula.EX or \
                self.op == Formula.EG or \
                self.op == Formula.A or \
                self.op == Formula.E or \
                self.op == Formula.G or \
                self.op == Formula.F or \
                self.op == Formula.X :
            return self.sub1.__easy_syntax_eq (g.sub1)
        elif self.op == Formula.OR or \
                self.op == Formula.LEQ or \
                self.op == Formula.AND or \
                self.op == Formula.EU or \
                self.op == Formula.U :
            return self.sub1.__easy_syntax_eq (g.sub1) and \
                    self.sub2.__easy_syntax_eq (g.sub2)
        else :
            raise Exception, 'Formula::__easy_syntax_eq, internal error'

    def __str__ (self) :
        if self.op == Formula.TRUE :
            return "true"
        elif self.op == Formula.FALSE :
            return "false"
        elif self.op == Formula.IS_FIREABLE :
            s = "is-firable("
            if len (self.atom_identifiers) > 5 :
                s += ", ".join (repr (t) for t in self.atom_identifiers[:5])
                s += ", ... %d more" % (len (self.atom_identifiers) - 5)
            else :
                s += ", ".join (repr (t) for t in self.atom_identifiers)
            return s + ")"
        elif self.op == Formula.LEQ :
            return "(" + str (self.sub1) + " <= " + str (self.sub2) + ")"
        elif self.op == Formula.NOT :
            return "(not " + str (self.sub1) + ")"
        elif self.op == Formula.OR :
            return "(" + str (self.sub1) + " or " + str (self.sub2) + ")"
        elif self.op == Formula.AND :
            return "(" + str (self.sub1) + " and " + str (self.sub2) + ")"
        elif self.op == Formula.EX :
            return "(EX " + str (self.sub1) + ")"
        elif self.op == Formula.EU :
            return "(E " + str (self.sub1) + " U " + str (self.sub2) + ")"
        elif self.op == Formula.EG :
            return "(EG " + str (self.sub1) + ")"
        elif self.op == Formula.TOKEN_COUNT :
            s = "token-count("
            if len (self.atom_identifiers) > 5 :
                s += ", ".join (repr (p) for p in self.atom_identifiers[:5])
                s += ", ... %d more" % (len (self.atom_identifiers) - 5)
            else :
                s += ", ".join (repr (p) for p in self.atom_identifiers)
            return s + ")"
        elif self.op == Formula.INT_CONST :
            return str (self.atom_int)
        elif self.op == Formula.A :
            return "(A " + str (self.sub1) + ")"
        elif self.op == Formula.E :
            return "(E " + str (self.sub1) + ")"
        elif self.op == Formula.G :
            return "(G " + str (self.sub1) + ")"
        elif self.op == Formula.F :
            return "(F " + str (self.sub1) + ")"
        elif self.op == Formula.X :
            return "(X " + str (self.sub1) + ")"
        elif self.op == Formula.U :
            return "(" + str (self.sub1) + " U " + str (self.sub2) + ")"
        else :
            raise Exception, 'Formula::__str__, internal error'

    @staticmethod
    def read (net, path, fmt='mcc15') :
        if fmt == 'mcc15' : return Formula.__read_mcc15 (net, path)
        raise Exception, "'%s': unknown input format" % fmt

    @staticmethod
    def __read_mcc15 (net, path) :
        print "smc: loading XML in memory (ElementTree.parse)"
        xmltree = xml.etree.ElementTree.parse (path)
        root = xmltree.getroot ()
        result = []
        for child in root :
            result.append (Formula.__read_mcc15_parse_property (net, child))
        return result

    @staticmethod
    def __read_mcc15_parse_property (net, xmltree) :
        f = Formula.__read_mcc15_parse_formula (net, xmltree.find ('{http://mcc.lip6.fr/}formula')[0])
        f.ident = xmltree.find ('{http://mcc.lip6.fr/}id').text
        return f

    @staticmethod
    def __read_mcc15_parse_formula (net, xmltree) :
        f = Formula ()
        #print 'smc: parsing xml', xmltree
        if xmltree.tag == '{http://mcc.lip6.fr/}all-paths' :
            f.op = Formula.A
            f.sub1 = Formula.__read_mcc15_parse_formula (net, xmltree[0])
        elif xmltree.tag == '{http://mcc.lip6.fr/}exists-path' :
            f.op = Formula.E
            f.sub1 = Formula.__read_mcc15_parse_formula (net, xmltree[0])
        elif xmltree.tag == '{http://mcc.lip6.fr/}globally' :
            f.op = Formula.G
            f.sub1 = Formula.__read_mcc15_parse_formula (net, xmltree[0])
        elif xmltree.tag == '{http://mcc.lip6.fr/}finally' :
            f.op = Formula.F
            f.sub1 = Formula.__read_mcc15_parse_formula (net, xmltree[0])
        elif xmltree.tag == '{http://mcc.lip6.fr/}next' :
            f.op = Formula.X
            f.sub1 = Formula.__read_mcc15_parse_formula (net, xmltree[0])
        elif xmltree.tag == '{http://mcc.lip6.fr/}until' :
            f.op = Formula.U
            f.sub1 = Formula.__read_mcc15_parse_formula (net, xmltree[0][0])
            f.sub2 = Formula.__read_mcc15_parse_formula (net, xmltree[1][0])
        elif xmltree.tag == '{http://mcc.lip6.fr/}negation' :
            f.op = Formula.NOT
            f.sub1 = Formula.__read_mcc15_parse_formula (net, xmltree[0])
        elif xmltree.tag == '{http://mcc.lip6.fr/}conjunction' :
            f.op = Formula.AND
            f.sub1 = Formula.__read_mcc15_parse_formula (net, xmltree[0])
            f.sub2 = Formula.__read_mcc15_parse_formula (net, xmltree[1])
        elif xmltree.tag == '{http://mcc.lip6.fr/}disjunction' :
            f.op = Formula.OR
            f.sub1 = Formula.__read_mcc15_parse_formula (net, xmltree[0])
            f.sub2 = Formula.__read_mcc15_parse_formula (net, xmltree[1])
        elif xmltree.tag == '{http://mcc.lip6.fr/}integer-le' :
            f.op = Formula.LEQ
            f.sub1 = Formula.__read_mcc15_parse_formula (net, xmltree[0])
            f.sub2 = Formula.__read_mcc15_parse_formula (net, xmltree[1])
        elif xmltree.tag == '{http://mcc.lip6.fr/}is-fireable' :
            f.op = Formula.IS_FIREABLE
            f.atom_identifiers = []
            if len (xmltree) > 1000 :
                print "smc: is-fireable XML tag with %d transition ids (!!)" % len (xmltree)
            nr = 0
            for xmlsub in xmltree :
                t = net.trans_lookup (xmlsub.text)
                if t == None :
                    raise Exception, "'%s': transition id not found in this net" % xmlsub.text
                f.atom_identifiers.append (t)
                nr += 1
                if nr % 10000 == 0 :
                    print "smc: loaded %d transition ids" % nr
        elif xmltree.tag == '{http://mcc.lip6.fr/}integer-constant' :
            f.op = Formula.INT_CONST
            f.atom_int = int (xmltree.text)
        elif xmltree.tag == '{http://mcc.lip6.fr/}tokens-count' :
            f.op = Formula.TOKEN_COUNT
            f.atom_identifiers = []
            if len (xmltree) > 1000 :
                print "smc: tokens-count XML tag with %d transition ids (!!)" % len (xmltree)
            nr = 0
            for xmlsub in xmltree :
                p = net.place_lookup (xmlsub.text)
                if p == None :
                    raise Exception, "'%s': place id not found in this net" % xmlsub.text
                f.atom_identifiers.append (p)
                nr += 1
                if nr % 10000 == 0 :
                    print "smc: loaded %d place ids" % nr
        else :
            raise Exception, "'%s': unable to handle XML tag, probably I cannot solve this formula" % xmltree.tag
        return f

__init_t = time.time ()

def db (*msg) :
    global __glob_t
    s = 'debug: %4.1f: ' % (time.time () - __init_t)
    s += ' '.join (str(x) for x in msg)
    sys.stderr.write (s + '\n')

def test1 () :
    n = ptnet.Net (True)
    try :
        f = open (net_path, 'r')
        n.read (f, fmt='pnml')
        f.close ()
    except IOError as (e, m) :
        raise Exception, "'%s': %s" % (net_path, m)

    # print transitions
    for t in n.trans :
        print t

def test2 () :
    n = ptnet.Net (True)
    n.m0['p1'] = 12
    n.m0['p2'] = 1200
    n.m0['p3'] = 33

    print 'aaaaaaaaaaaaaaaaa'
    m1 = n.m0
    m1.formulas_sat.add (11111)
    m2 = m1.clone ()
    m2.formulas_sat.add (2222222)
    print hash (m1)
    print hash (m2)
    print id (m1)
    print id (m2)
    print m1 == m2

    print 'bbbbbbbbbbbbbbbbbb'
    d = {}
    d[m1] = 1111111111;
    d[m2] = 2222222;
    for k,v in d.items () :
        print k
        print v
    return

    print 'ccccccccccccccccccccc'
    g = networkx.DiGraph ()
    g.add_edge (m1, m2)
    print m1 in g
    print m1 in g
    print len (g)
    return

    print 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxx'
    print 'initial marking'
    print n.m0
    new = n.m0.clone ()
    print new
    print 'equal before modification'
    print new == n.m0
    for p in n.m0 :
        assert (new[p] == n.m0[p])
    for p in new :
        assert (new[p] == n.m0[p])
    n.m0.fully_expanded = False
    new.fully_expanded = True
    new['aaaaaaaaaaaa'] = 123
    print 'equal after modification'
    print new == n.m0
    print new, n.m0
    print 'aaaaaaaaaaaaaaaaa'
    g.add_edge (n.m0, new)
    neww = n.m0.clone ()
    neww.formulas_sat.add (123)
    neww.fully_expanded (123)
    g.add_edge (new, neww)
    print len (g)
    return

    print 'enabled'
    a = n.enabled (n.m0)
    print 'the first'
    t = list (a) [0]
    print t
    print 'fire the first'
    print n.fire (n.m0, t)


def test3 () :

    # a mutual exclusion example
    net = ptnet.Net (True)
    idle0 = ptnet.Place ('idle0')
    req0 = ptnet.Place ('req0')
    crit0 = ptnet.Place ('crit0')

    idle1 = ptnet.Place ('idle1')
    req1 = ptnet.Place ('req1')
    crit1 = ptnet.Place ('crit1')

    sem = ptnet.Place ('sem')

    for p in [idle0, req0, crit0, idle1, req1, crit1, sem] :
        net.places.append (p)

    try0 = ptnet.Transition ('try0')
    enter0 = ptnet.Transition ('enter0')
    exit0 = ptnet.Transition ('exit0')
    try1 = ptnet.Transition ('try1')
    enter1 = ptnet.Transition ('enter1')
    exit1 = ptnet.Transition ('exit1')

    for t in [try0, enter0, exit0, try1, enter1, exit1] :
        net.trans.append (t)
    net.m0[idle0] = 1
    net.m0[idle1] = 1
    net.m0[sem] = 1

    try0.pre_add (idle0)
    try0.post_add (req0)
    enter0.pre_add (req0)
    enter0.pre_add (sem)
    enter0.post_add (crit0)
    exit0.pre_add (crit0)
    exit0.post_add (idle0)
    exit0.post_add (sem)

    try1.pre_add (idle1)
    try1.post_add (req1)
    enter1.pre_add (req1)
    enter1.pre_add (sem)
    enter1.post_add (crit1)
    exit1.pre_add (crit1)
    exit1.post_add (idle1)
    exit1.post_add (sem)

    net.write (sys.stdout, 'pep')
    return 0

    # testing the CTL model checking algorithms
    f1 = Formula (Formula.IS_FIREABLE)          # is_firable (enter0)
    f1.atom_identifiers = [enter0]  
    f2 = Formula (Formula.TRUE)                 # true
    f3 = Formula (Formula.EU, f2, f1)           # EF f1
    f4 = Formula (Formula.EG, f1)               # EG f1
    f5 = Formula (Formula.EG, f2)               # EG true
    f6 = Formula (Formula.FALSE)                # false
    f7 = Formula (Formula.INT_CONST)
    f7.atom_int = 1
    f8 = Formula (Formula.TOKEN_COUNT)
    f8.atom_identifiers = [crit1]
    f9 = Formula (Formula.LEQ, f7, f8)          # 1 <= crit1
    f10 = Formula (Formula.NOT, f9)             # not (1 <= crit1)
    f11 = Formula (Formula.OR, f9, f10)         # f9 or not f9
    f12 = Formula (Formula.EX, f9)              # EX f9
    f13 = Formula (Formula.TOKEN_COUNT)
    f13.atom_identifiers = [idle1]
    f14 = Formula (Formula.LEQ, f7, f13)        # 1 <= idle1
    f15 = Formula (Formula.IS_FIREABLE)         # is_firable (exit0)
    f15.atom_identifiers = [exit0]  
    f16 = Formula (Formula.EU, f14, f15)        # E (1 <= idle1) U is_firable (exit0)
    f17 = Formula (Formula.EG, f14)             # EG (1 <= idle1)
    f18 = Formula (Formula.EU, f9, f17)         # E (1 <= crit1) U EG (1 <= idle1)
    f19 = Formula (Formula.OR, f9, f14)         # (1 <= crit1) or (1 <= idle1)
    f20 = Formula (Formula.EG, f19)             # EG (1 <= crit1) or (1 <= idle1)
    f21 = Formula (Formula.NOT, f3)             # not EF f1

    print 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'
    f = f21
    explo = BoundedSearch (net, f)
    result = explo.run ()
    explo.debug_print_states ([f])
    #print 'saaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaat'
    #explo.debug_print_states_sat (f)
    #print 'undeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeef'
    #explo.debug_print_states_undef (f)
    print f
    print result

    # testing the formula rewrite code
    f22 = Formula (Formula.E, Formula (Formula.F, f1))      # EF f1
    f22 = Formula (Formula.A, Formula (Formula.X, f1))      # AX f1
    f23 = Formula (Formula.A, Formula (Formula.G, f1))      # AX f1
    f24 = Formula (Formula.A, Formula (Formula.F, f1))      # AX f1
    f25 = Formula (Formula.A, Formula (Formula.U, f1, f9))  # A (f1 U f9)
    f26 = Formula (Formula.EG, f22)                         # EG EF f1

    print 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'
    f = f26
    print 'before'
    print f
    print 'after'
    f.rewrite ()
    print f


def test4 () :
    g = networkx.DiGraph ();

    m1 = ptnet.Marking ()
    m1['p'] = 123
    m1['q'] = 2
    m2 = m1.clone ()
    m3 = m1.clone ()
    m2['z'] = 12333

    g.add_node (m1, original=m1)
    g.add_node (m2, original=m2)
    g.add_edge (m3, m2, trans=1235)
    print repr (m1)
    print repr (g.node[m1]['original'])
    print g[m3][m2]

    print g.out_edges (m1, True)

def test5 () :
    import xml.dom.minidom
    dom = xml.dom.minidom.parse ("./formula.xml")
    print dom
    l = dom.getElementsByTagName ('property')
    for elem in l :
        print elem
        ll = elem.getElementsByTagName ('id')
        assert (len (ll) == 1)
        assert (len (ll[0].childNodes) == 1)
        print ll[0]
        print ll[0].childNodes[0]
        ll = elem.getElementsByTagName ('formula')
        assert (len (ll) == 1)
        print ll[0]

def percent (i, n) :
    return i * 100.0 / n

def resource_usage () :
    rusage = resource.getrusage (resource.RUSAGE_SELF)
    m = rusage.ru_maxrss
    t = time.time () - __init_t

    tu = 's'
    if t >= 60 :
        tu = 'm'
        t /= 60.0
        if t >= 60 :
            tu = 'h'
            t /= 60.0

    mu = 'K'
    if m >= 1024 :
        mu = 'M'
        m /= 1024
        if m >= 1024 :
            mu = 'G'
            m /= 1024

    print "smc: resources: wall time %.1f%s maxrss %d%s" % (t, tu, m, mu)

def parse () :
    p = argparse.ArgumentParser (usage = __doc__, add_help=False)
    p.add_argument ("-h", "--help", action="store_true")
    p.add_argument ("--max-depth", type=int, default=-1)
    p.add_argument ("--max-states", type=int, default=2000)
    p.add_argument ("--mcc15-stop-after", type=int, default=-1)
    p.add_argument ("--explore-only", action="store_true")
    p.add_argument ("--use10", action="store_true")

    p.add_argument ('net_path')
    p.add_argument ('formula_path')

    args = p.parse_args ()
    if args.help :
        print __doc__
        sys.exit (0);

    if args.max_depth < 0 :
        args.max_depth = sys.maxint
    if args.max_states < 0 :
        args.max_states = sys.maxint
    if args.mcc15_stop_after < 0 :
        args.mcc15_stop_after = sys.maxint
    return args

def main () :

    #test3 ()
    #return

    args = parse ()
    print "smc: command arguments", args
    net = ptnet.Net (True)
    size = os.path.getsize (args.net_path) / (1024 * 1024.0)
    print "smc: loading net file '%s' (%.1fM)" % (args.net_path, size)
    try :
        f = open (args.net_path, 'r')
        net.read (f, fmt='pnml')
        f.close ()
    except IOError as (e, m) :
        raise Exception, "'%s': %s" % (args.net_path, m)
    print "smc: done, %d transitions, %d places" % (len (net.trans), len (net.places))
    print "smc: first 3 transitions are:"
    i = 0
    for t in net.trans :
        print "smc:", t
        i += 1
        if i >= 3 : break
    if args.explore_only :
        formulas = [Formula (Formula.FALSE)]
    else :
        size = os.path.getsize (args.formula_path) / (1024 * 1024.0)
        print "smc: laoding formula file '%s' (%.1fM)" % (args.formula_path, size)
        formulas = Formula.read (net, args.formula_path)
        print "smc: done,", len (formulas), "formulas"

    have10 = (args.max_states >= 100) and args.use10
    explo10 = BoundedSearch (net)
    explo10.max_depth = sys.maxint
    explo10.max_states = 10
    if have10 :
        explo10.build_state_space ()

    explo = BoundedSearch (net)
    explo.max_depth = args.max_depth
    explo.max_states = args.max_states
    explo.build_state_space ()

    results = []
    stats_sat, stats_unsat, stats_undef = 0, 0, 0
    for formula in formulas :
        print "smc:", "=" * 75
        print "smc: processing formula '%s'" % formula.ident

        try :
            # try to solve the formula on only 10 states (very fast)
            result = BoundedSearch.RESULT_UNDEF
            if have10 :
                print "smc: first pass on 10 states"
                result = explo10.verify (formula)
                if result == BoundedSearch.RESULT_UNDEF :
                    print "smc: unable to decide with 10 states"
                    print "smc:", "-" * 20

            # if we don't manage to solve it, try on more states (slower)
            if result == BoundedSearch.RESULT_UNDEF :
                result = explo.verify (formula)

        except KeyboardInterrupt :
            print 'smc: interrupted by the user (cltr+c), doing an ordered termination'
            break;

        print "smc: verification terminated with result:"
        print "smc:", result
        results.append (result)

        if result == BoundedSearch.RESULT_SAT : stats_sat += 1
        if result == BoundedSearch.RESULT_UNSAT : stats_unsat += 1
        if result == BoundedSearch.RESULT_UNDEF : stats_undef += 1

        if stats_undef >= args.mcc15_stop_after : break

    print "smc:", "=" * 75
    print "smc: summary of the results:\n"
    for i in range (len (results)) :
        print "%-5s - %s" % (results[i], formulas[i].ident)

    print "\nsmc: state-space:", \
        explo.stats_nr_states, "states,", \
        explo.stats_nr_states - explo.stats_nr_states_fe, "not fully expanded,", \
        explo.stats_nr_edges, "edges,", \
        explo.stats_depth, "complete steps deep"
    print "smc: state-space: limits: maximum depth:", explo.max_depth
    print "smc: state-space: limits: maximum nr. of states:", explo.max_states
    print "smc: state-space: construction terminated due to: ", explo.stats_stop_reason
    stats_all = stats_sat + stats_unsat + stats_undef
    assert (len (formulas) >= len (results))
    assert (stats_all == len (results))
    if stats_all == 0 : stats_all = 1

    print "smc: formulas: among the verified: %.1f%% SAT, %.1f%% UNSAT, %.1f%% ???" % \
            (percent (stats_sat, stats_all), \
            percent (stats_unsat, stats_all), \
            percent (stats_undef, stats_all))

    print "smc: formulas: %d unverified, %d SAT, %d UNSAT, %d ???; total %d" % \
            (len (formulas) - stats_all, stats_sat, stats_unsat, stats_undef, len (formulas))
    resource_usage ()
    if explo.stats_nr_states == explo.stats_nr_states_fe and stats_undef > 0 :
        print "smc: WARNING: explored the full state space but some was unable to decide some formulas; is this a bug?"

if __name__ == '__main__' :
    #main ()
    #test3 ()
    #sys.exit (0)
    try :
        main ()
        pass
    except KeyboardInterrupt :
        print 'smc: interrupted by the user (cltr+c)'
        sys.exit (1)
    except Exception as e :
        print 'smc: error: %s' % str (e)
        sys.exit (1)
    sys.exit (0)

# vi:ts=4:sw=4:et:
