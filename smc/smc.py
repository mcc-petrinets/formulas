#!/usr/bin/env python

''' smc PNML FORMULA

The Simple Model Checker is a bounded CTL model checker

'''

try :
    import os
    import sys
    import time
    #import math
    #import argparse
    import networkx
    #import tempfile
    #import subprocess
    import xml.dom.minidom
    import xml.etree.ElementTree
except Exception, e:
    print 'ERROR!'
    print 'It seems that your python installation is missing some package.'
    print 'This tool requires, among others, argparse, networkx, tempfile, and subprocess'
    print 'The runtime reported the following error:\n\n', str (e)
    print 'You might want to use "easy_install --user PACKAGE"'
    sys.exit (1)
finally :
    if sys.version_info < (2, 6, 1) :
        print '\n\nWARNING: This tool requires Python v.2.6.1 at least!!\n'
    sys.path.append (os.path.dirname (sys.argv[0]) + '/../lib/')

import ptnet

def sgl (s) :
    return (list (s))[0]

class BoundedSearch :

    # outcome of the CTL verification algorithm
    RESULT_SAT   = 'SAT'
    RESULT_UNSAT = 'UNSAT'
    RESULT_UNDEF = 'UNDEF'

    def __init__ (self, net, formula) :
        self.net = net
        self.formula = formula
        self.max_depth = 3
        self.max_states = 7
        self.states = networkx.DiGraph ()
        self.init_state = net.m0
        self.__new_mark = 0

    def new_mark (self) :
        self.__new_mark += 1
        return self.__new_mark

    def find_and_insert (self, marking) :
        if marking in self.states :
            return self.states.node[marking]['org']
        self.states.add_node (marking, org=marking)
        return marking

    def build_state_space (self) :
        assert (self.init_state == self.net.m0)
        curr_depth = 0
        self.find_and_insert (self.init_state)
        layer = set ([self.init_state])
        next_layer = set ()

        # bread-first search on the state space
        print "smc: constructing state space"
        print "smc: limits: maximum depth:", self.max_depth
        print "smc: limits: maximum nr. of states:", self.max_states
        while (len (self.states) < self.max_states
                and curr_depth < self.max_depth
                and len (layer) >= 1) :
            #print 'smc: build: start processing', len (layer), 'nodes at depth', curr_depth
            #print 'smc: build: so far', len(self.states), 'nodes in the graph'
            for marking in layer :
                #print 'build: expanding marking', repr (marking)
                if (marking.fully_expanded) : continue
                for t in self.net.enabled (marking) :
                    new_marking = self.net.fire (marking, t)
                    new_canon_marking = self.find_and_insert (new_marking)
                    #print 'smc: build: - adding edge to', repr (new_canon_marking), 'trans', repr (t)
                    self.states.add_edge (marking, new_canon_marking, trans=t)
                    next_layer.add (new_canon_marking)
                marking.fully_expanded = True
                if len (self.states) >= self.max_states :
                    curr_depth -= 1 # avoids bad reporting of reason 
                    break
            curr_depth += 1
            layer = next_layer
            next_layer = set ()

        print "smc: done,", \
            self.states.number_of_nodes (), "states,", \
            self.states.number_of_edges (), "edges,", \
            curr_depth, "steps deep"
        if curr_depth >= self.max_depth :
            print "smc: state-space construction terminated due to depth"
        if len (self.states) >= self.max_states :
            print "smc: state-space construction terminated due to number of states"

        for s in self.states :
            assert (s.fully_expanded or len (self.states.successors (s)) == 0)

    def debug_print_states (self, formulas) :
        for s in self.states :
            self.__debug_print_state (s, formulas)

    def __debug_print_state (self, s, formulas) :
        print "%r, %d successors%s" % (s, len (self.states.successors (s)), \
                "" if s.fully_expanded else ", not fully expanded")
        for ss in self.states.successors (s) :
            print "  edge  %-10r %r" % (self.states[s][ss]['trans'], ss)
        print "  sat  ", [str (f) for f in formulas if s.is_sat (f)]
        print "  unsat", [str (f) for f in formulas if s.is_unsat (f)]
        print "  undef", [str (f) for f in formulas if s.is_undef (f)]

    def debug_print_states_sat (self, formula) :
        for s in self.states :
            if s.is_sat (formula) :
                self.__debug_print_state (s, [formula])

    def debug_print_states_undef (self, formula) :
        for s in self.states :
            if s.is_undef (formula) :
                self.__debug_print_state (s, [formula])

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

        for marking in self.states :
            found = False
            for next_marking in self.states.successors (marking) :
                if self.states[marking][next_marking]['trans'] in \
                        formula.atom_identifiers :
                    found = True
                    break
            if found :
                marking.formulas_sat.add (formula)
            elif not marking.fully_expanded :
                marking.formulas_undef.add (formula)

    def __label_states_leq (self, formula) :
        print "smc: labelling formula:", formula
        assert (formula.op == Formula.LEQ)
        assert (formula.sub1 != None)
        assert (formula.sub2 != None)

        for marking in self.states :
            a = self.__label_states_eval_int (marking, formula.sub1)
            b = self.__label_states_eval_int (marking, formula.sub2)
            if a <= b :
                marking.formulas_sat.add (formula)

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

        for marking in self.states :
            # if the subformula is UNDEF, then the NOT is also UNDEF
            if formula.sub1 in marking.formulas_undef :
                marking.formulas_undef.add (formula)
                continue
            # otherwise we can decide the NOT
            if formula.sub1 not in marking.formulas_sat :
                marking.formulas_sat.add (formula)

    def __label_states_or (self, formula) :
        print "smc: labelling formula:", formula
        assert (formula.op == Formula.OR)
        assert (formula.sub1 != None)
        assert (formula.sub2 != None)

        for marking in self.states :
            # if at least one is SAT, then the OR is SAT
            if marking.is_sat (formula.sub1) or marking.is_sat (formula.sub2) :
                marking.formulas_sat.add (formula)
                continue

            # if both are UNSAT, then the OR is UNSAT
            if marking.is_unsat (formula.sub1) and marking.is_unsat (formula.sub2) :
                continue

            # otherwise the OR is UNDEF
            marking.formulas_undef.add (formula)

    def __label_states_and (self, formula) :
        print "smc: labelling formula:", formula
        assert (formula.op == Formula.AND)
        assert (formula.sub1 != None)
        assert (formula.sub2 != None)

        for marking in self.states :
            # if both are SAT, then the AND is SAT
            if marking.is_sat (formula.sub1) and marking.is_sat (formula.sub2) :
                marking.formulas_sat.add (formula)
                continue

            # if one is UNSAT, then the AND is UNSAT
            if marking.is_unsat (formula.sub1) or marking.is_unsat (formula.sub2) :
                continue

            # otherwise the OR is UNDEF
            marking.formulas_undef.add (formula)

    def __label_states_ex (self, formula) :
        print "smc: labelling formula:", formula
        assert (formula.op == Formula.EX)
        assert (formula.sub1 != None)

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
                continue
            if not marking.fully_expanded or found_undef :
                marking.formulas_undef.add (formula)

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
        #           not in f1-SCC but there is s' with f1 SAT   SAT
        #           for all s', f1 UNSAT and s fully exp.       UNSAT
        #           else                                        UNDEF
        # UNSAT                                                 UNSAT
        # UNDEF     for all s', f1 UNSAT and s fully exp.       UNSAT
        #           else                                        UNDEF
        # ========= =========================================== =======

        msat = self.new_mark ()
        munsat = self.new_mark ()

        self.__label_states_eg_mark_sat (formula.sub1, msat)
        self.__label_states_eg_mark_unsat (formula.sub1, munsat)

        for s in self.states :
            if s.m == msat :
                s.formulas_sat.add (formula)
            elif s.m != munsat :
                s.formulas_undef.add (formula)

    def __label_states_eg_mark_sat (self, f, msat) :
        work = []
        subg = self.states.subgraph (s for s in self.states if s.is_sat (f))
        sccs = networkx.algorithms.strongly_connected_components (subg)
        for scc in sccs :
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

        self.__label_states_eu_mark_sat (formula, msat)
        self.__label_states_eu_mark_unsat (formula, munsat)

        for s in self.states :
            if s.m == msat :
                s.formulas_sat.add (formula)
            elif s.m != munsat :
                s.formulas_undef.add (formula)

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

    def run (self) :

        # rewrite the formula so it only uses the first 8 operators:
        # NOT, OR, EX, EU, EG, and atomic propositions (3)
        self.formula.rewrite ()

        # simplify the formula, identify common subformulas
        self.formula.simplify ()

        # construct a bounded fragment of the state space
        self.build_state_space ()

        # explore the syntax tree of the formula, starting from the
        # smallest subformulas and up to the larger ones, and labell every
        # state in self.states with the subformulas satisfied by that state
        print "smc: labelling states, formula:", self.formula
        self.label_states (self.formula)

        # at this point, we can decide satisfiability of the formula under
        # this fragment of the state space iff the formula labels the
        # initial state
        print "smc: labelling done!"
        if self.formula in self.init_state.formulas_undef :
            return BoundedSearch.RESULT_UNDEF
        if self.formula in self.init_state.formulas_sat :
            return BoundedSearch.RESULT_SAT
        else :
            return BoundedSearch.RESULT_UNSAT

class Formula :
    # types of nodes in the formula tree 
    TRUE         = -1
    FALSE        = 0
    IS_FIREABLE  = 7
    LEQ          = 8
    NOT          = 1
    OR           = 2
    AND          = 3
    EX           = 4
    EU           = 5
    EG           = 6
    # ..............
    TOKEN_COUNT  = 9
    INT_CONST    = 10
    A            = 11
    E            = 12
    G            = 13
    F            = 14
    X            = 16
    U            = 17

    def __init__ (self, op = None, sub1 = None, sub2 = None) :
        self.ident = None
        self.op = op
        self.sub1 = sub1
        self.sub2 = sub2
        self.atom_identifiers = None
        self.atom_int = None

    def rewrite (self) :
        pass

    def simplify (self) :
        pass

    def __str__ (self) :
        if self.op == Formula.TRUE :
            return "true"
        elif self.op == Formula.FALSE :
            return "false"
        elif self.op == Formula.IS_FIREABLE :
            s = "is-firable("
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
        else :
            assert (False)

    @staticmethod
    def read (net, path, fmt='mcc15') :
        if fmt == 'mcc15' : return Formula.__read_mcc15 (net, path)
        raise Exception, "'%s': unknown input format" % fmt

    @staticmethod
    def __read_mcc15 (net, path) :
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
            for xmlsub in xmltree :
                t = net.trans_lookup (xmlsub.text)
                if t == None :
                    raise Exception, "'%s': transition id not found" % xmlsub.text
                f.atom_identifiers.append (t)
        elif xmltree.tag == '{http://mcc.lip6.fr/}integer-constant' :
            f.op = Formula.INT_CONST
            f.atom_int = int (xmltree.text)
        elif xmltree.tag == '{http://mcc.lip6.fr/}tokens-count' :
            f.op = Formula.TOKEN_COUNT
            f.atom_identifiers = []
            for xmlsub in xmltree :
                p = net.place_lookup (xmlsub.text)
                if p == None :
                    raise Exception, "'%s': transition id not found" % xmlsub.text
                f.atom_identifiers.append (p)
        else :
            assert (False)
        return f

__init_t = time.time ()

def db (*msg) :
    global __glob_t
    s = 'debug: %4.1f: ' % (time.time () - __init_t)
    s += ' '.join (str(x) for x in msg)
    sys.stderr.write (s + '\n')

def opendef (p, s, m) :
    if s == '-' :
        return sys.stdin if m == 'r' else sys.stdout
    try :
        return open (s, m)
    except IOError as (e, t) :
        p.error ("'%s': %s" % (s, t))
    except Exception, e:
        p.error ("'%s': %s" % (s, str (e)))

def usage () :
    print __doc__
    sys.exit (0);

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
    m2 = m1.copy ()
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
    new = n.m0.copy ()
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
    neww = n.m0.copy ()
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

    #net.write (sys.stdout, 'dot')

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

    print 'xxxxxxxxxxxxxxxxxxxxxxxxxxxx'
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


def test4 () :
    g = networkx.DiGraph ();

    m1 = ptnet.Marking ()
    m1['p'] = 123
    m1['q'] = 2
    m2 = m1.copy ()
    m3 = m1.copy ()
    m2['z'] = 12333

    g.add_node (m1, original=m1)
    g.add_node (m2, original=m2)
    g.add_edge (m3, m2, trans=1235)
    print repr (m1)
    print repr (g.node[m1]['original'])
    print g[m3][m2]

    print g.out_edges (m1, True)

def test5 () :
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

def main () :
    if (len (sys.argv) != 3) : usage ()
    net_path = sys.argv[1]
    formula_path = sys.argv[2]

    net = ptnet.Net (True)
    print "smc: loading net file '%s'" % net_path
    try :
        f = open (net_path, 'r')
        net.read (f, fmt='pnml')
        f.close ()
    except IOError as (e, m) :
        raise Exception, "'%s': %s" % (net_path, m)
    print "smc: done,", len (net.trans), "transitions", len (net.places), "places"
    print "smc: first 3 transitions are:"
    i = 0
    for t in net.trans :
        print "smc:", t
        i += 1
        if i >= 3 : break
    print "smc: laoding formula file '%s'" % formula_path
    formulas = Formula.read (net, formula_path)
    print "smc: done,", len (formulas), "formulas"
    for formula in formulas :
        print formula

if __name__ == '__main__' :
    main ()
    try :
        #main ()
        print ""
    except KeyboardInterrupt :
        print 'smc: interrupted'
        sys.exit (1)
    except Exception as e :
        print 'smc: error: %s' % str (e)
        sys.exit (1)
    sys.exit (0)

# vi:ts=4:sw=4:et:
