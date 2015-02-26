#!/usr/bin/env python

''' smc PNML FORMULA

The Simple Model Checker is a bounded CTL model checker

'''

try :
    import os
    import sys
    import time
    import math
    import argparse
    import networkx
    import tempfile
    import subprocess
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

    # outcome of the verification
    RESULT_SAT   = 1
    RESULT_UNSAT = 2
    RESULT_UNKN  = 3

    def __init__ (self, net, formula) :
        self.net = net
        self.formula = formula
        self.max_depth = 4
        self.max_states = 1000
        self.states = networkx.DiGraph ()
        self.init_state = net.m0

    def build_state_space (self) :
        assert (self.init_state == self.net.m0)
        curr_depth = 0
        nr_states = 1
        


        

    def label_states (self, formula) :
        pass

    def run (self) :

        # rewrite the formula so it only uses the first 8 operator:
        # NOT, OR, EX, EU, EG, and atomic propositions (3)
        self.formula.rewrite ()

        # construct a bounded fragment of the state space
        self.build_state_space ()

        # explore the syntax tree of the formula, starting from the
        # smallest subformulas and up to the larger ones, and labell every
        # state in self.states with the subformulas satisfied by that state
        self.label_states (self.formula)

        # at this point, the formula is satisfiable under this fragment of
        # the state space iff it labels the initial state
        if self.formula in self.init_state.formulas_unkn :
            return RESULT_UNKN
        if self.formula in self.init_state.formulas_sat :
            return RESULT_SAT
        else :
            return RESULT_UNSAT

class Formula :
    # next identifier for a newly created formula
    next_id = 1

    # types of nodes in the formula tree 
    NOT          = 1
    OR           = 2
    EX           = 3
    EU           = 4
    EG           = 5
    IS_FIRABLE   = 6
    TOKEN_COUNT  = 7
    INT_CONST    = 8
    # ..............
    AND          = 9
    A            = 10
    E            = 11
    G            = 12
    F            = 13
    X            = 14

    def __init__ (self, op, sub1 = None, sub2 = None) :
        self.id = next_id
        next_id += 1
        self.op = op
        self.sub1 = sub1
        self.sub2 = sub2
        self.atom_identifiers = None
        self.atom_int = 0

    def label_state (self, marking) :
        pass

    def rewrite (self) :
        pass

class Cna :
    def __init__ (self, opt) :
        self.opt = {}

        # define option switches
        self.opt['deadlock'] = opt.deadlock
        self.opt['coverability'] = opt.cover != None
        self.opt['assert'] = True
        self.opt['verbose'] = opt.verbose != None
        self.opt['tree'] = 0
        self.opt['seq'] = False
        self.opt['log'] = False
        self.opt['pair'] = False
        self.opt['nocy'] = False
        self.opt['bin'] = False
        self.opt['unary'] = False
        self.opt['trans'] = False
        self.opt['sccred'] = False
        self.opt['stb'] = False
        self.opt['sub'] = False
        if opt.reduce :
            for o in opt.reduce :
                if '-tree' in o :
                    self.opt['tree'] = int ((o.split ('-'))[0])
                else :
                    self.opt[o] = True

        # validate them and set defaults
        n = 0
        if self.opt['tree'] != 0 : n += 1
        if self.opt['seq'] : n += 1
        if self.opt['log'] : n += 1
        if self.opt['pair'] : n += 1
        if n == 0 : self.opt['tree'] = 4
        if n > 1 : raise Exception, 'At most one of k-tree, seq, log or pair!'
        if self.opt['tree'] == 1 : raise Exception, 'Use k > 1 for k-trees!'

        n = 0
        if self.opt['nocy'] : n += 1
        if self.opt['bin'] : n += 1
        if self.opt['unary'] : n += 1
        if self.opt['trans'] : n += 1
        if n == 0 : self.opt['bin'] = True
        if n > 1 : raise Exception, 'At most one of nocy, bin, unary or trans!'

        if self.opt['stb'] and self.opt['coverability'] :
            raise Exception, \
                    'Stubborn reduction only works with deadlock checking'

        # declare additional fields
        self.phi = Cnf ()
        self.u = ptnet.unfolding.Unfolding (self.opt['assert'])
        self.result = {}
        self.stubborn = {}
        self.pred = {}

        # load the model
        #db ('load cuf')
        try :
            self.u.read (opt.infile, fmt='cuf')
            if opt.infile != sys.stdin : opt.infile.close ()
        except IOError as (e, m) :
            raise Exception, "input file: %s" % m
        except Exception, e:
            raise Exception, "input file: %s" % str (e)

        # declare the reductions used in the construction of the formula
        s = ''
        for k in self.opt :
            if k in ['tree', 'verbose', 'assert', 'deadlock', 'coverability'] :
                continue
            if self.opt[k] : s += k + ' '
        if self.opt['tree'] != 0 : s += '%d-tree' % self.opt['tree']
        self.result['reductions'] = s

        # list of places for coverability
        self.opt['places'] = \
                self.__find_places (opt.cover if opt.cover else [])

    def deadlock (self) :
        # nothing to do if there is zero cutoffs
        #db ('self.u.nr_black = ', self.u.nr_black)
        if self.u.nr_black == 0 :
            self.phi.add ([self.phi.var (1)])
            return self.phi

        # causal closure
        #db ('deadlock causal')
        self.__causal ()
        self.result['event variables'] = len (self.phi.varmap)

        # symmetric conflicts
        #db ('sym')
        self.__sym ()

        # asymmetric conflicts
        #db ('asym')
        self.__asym ()

        # marking and disabled
        #db ('dis')
        self.__dis ()

        self.result['variables'] = len (self.phi.varmap)
        self.result['clauses'] = len (self.phi.clsset)
        return self.phi

    def cover (self) :
        self.__causal ()
        self.result['event variables'] = len (self.phi.varmap)
        self.__sym ()
        self.__asym ()
        self.__cover ()
        self.result['variables'] = len (self.phi.varmap)
        self.result['clauses'] = len (self.phi.clsset)
        return self.phi

    def __causal (self) :
        # a configuration is causally closed ...
        for e in self.u.events :
            if e.isblack or self.__is_stubborn (e) : continue
            s = self.__pred (e)
            atm = -self.phi.var (e)
            if not s : continue
            for ep in s :
                self.phi.add ([atm, self.phi.var (ep)])

    def __sym_ismaximal (self, c) :
        # c has at least two non-black events
        if not self.opt['sub'] : return True
        m = self.u.m
        self.u.m += 1
        l = [e for e in c.post if not e.isblack]
        n = len (l)
        for e in l[1:] :
            for cp in e.pre :
                if cp == c : continue
                if cp.m != m :
                    cp.m = m
                    cp.c = 1
                else :
                    cp.c += 1
        for cp in l[0].pre :
            if cp == c or cp.m != m : continue
            cp.c += 1
            if cp.c == n :
                np = len ([e for e in cp.post if not e.isblack])
#                if n < np or cp.nr > c.nr : db ('symm_not_max', c, cp)
                if n < np or cp.nr > c.nr : return False
        return True

    def __sym (self) :
        # ... and has no symmetric conflict
        for c in self.u.conds[1:] :
            l = [e for e in c.post if not e.isblack]
            if len (l) < 2 : continue
            if not self.__sym_ismaximal (c) : continue
            ll = [self.phi.var (e) for e in l]
            if self.opt['tree'] > 1 :
                self.phi.amo_ktree (ll, self.opt['tree'])
            elif self.opt['seq'] :
                self.phi.amo_seq (ll)
            elif self.opt['log'] :
                self.phi.amo_bin (ll)
            elif self.opt['pair'] :
                self.phi.amo_pairwise (ll)
            else :
                raise Exception, 'sym internal error'

    def __asym (self) :
        if self.opt['nocy'] : return

        # generate asymmetric conflict graph (without sym. conflicts)
        g = self.u.asym_graph (False)

        # search for sccs
        sccs = networkx.algorithms.strongly_connected_components (g)
        sccs = [x for x in sccs if len (x) >= 2]

        sizes = []
        for x in sccs :
            h = g.subgraph (x)
            m = h.size ()
            if self.opt['sccred'] :
                h = self.__reduce (h)
            sizes.append ((len (x), m, len (h), h.size ()))
            self.__asym_scc (h)
        self.result['sccs'] = repr (sizes)

    def __asym_scc (self, g) :
        if self.opt['trans'] :
            self.__asym_scc_trans (g)
            return

        k = int (math.ceil (math.log (len (g), 2)))
#        k = 1
#        db ('k', k);
        for e in g :
            for ep in g[e] :
                if self.opt['bin'] :
                    self.__asym_scc_bin_ord (e, ep, k)
                elif self.opt['unary'] :
                    self.__asym_scc_una_ord (e, ep, len (g))
                else :
                    raise Exception, 'asym internal error'
        if self.opt['unary'] and len (g[e]) :
            self.__asym_scc_una (e, len (g))

    def __asym_scc_bin_ord (self, a, b, k) :
        # a > b
#        db (repr (a), repr (b), k)
        ai = self.phi.var ((a, 0))
        bi = self.phi.var ((b, 0))
        ci1 = self.phi.var ((a, b, 0))

        self.phi.add ([ai, -ci1])
        self.phi.add ([-ai, -bi, -ci1])
#        self.phi.add ([-ai, bi, ci]) # optim.

        for i in xrange (1, k) :
            ai = self.phi.var ((a, i))
            bi = self.phi.var ((b, i))
            ci = self.phi.var ((a, b, i))

            self.phi.add ([ai, -bi, -ci])
            self.phi.add ([-ai, -bi, -ci1, ci])
            self.phi.add ([-ai, -bi, ci1, -ci])
            self.phi.add ([ai, bi, -ci1, ci])
            self.phi.add ([ai, bi, ci1, -ci])
#            self.phi.add ([-ai, bi, ci]) # optim.
            ci1 = ci

        if self.__is_stubborn (a) :
            l = [-self.phi.var (e) for e in self.__pred (a)]
        else :
            l = [-self.phi.var (a)]

        if self.__is_stubborn (b) :
            l += [-self.phi.var (e) for e in self.__pred (b)]
        else :
            l += [-self.phi.var (b)]
        l.append (self.phi.var ((a, b, k - 1)))
        self.phi.add (l)

    def __asym_scc_trans (self, g) :
        for e in g :
            if self.__is_stubborn (e) :
                l = [-self.phi.var (ep) for ep in self.__pred (e)]
            else :
                l = [-self.phi.var (e)]

            for ep in g[e] :
                if self.__is_stubborn (ep) :
                    l2 = [-self.phi.var (epp) for epp in self.__pred (ep)]
                else :
                    l2 = [-self.phi.var (ep)]
                self.phi.add (l + l2 + [self.phi.var ((e, ep))])

        for e in g :
            for e1 in g :
                if e == e1 : continue
                atm = - self.phi.var ((e, e1))
                atm1 = - self.phi.var ((e1, e))
                self.phi.add ([atm, atm1])

        for e in g :
            for e1 in g :
                if e == e1 : continue
                atm = - self.phi.var ((e, e1))
                for e2 in g[e1] :
                    if e == e2 or e2 in g[e] : continue
                    atm1 = -self.phi.var ((e1, e2))
                    atm2 = self.phi.var ((e, e2))
                    self.phi.add ([atm, atm1, atm2])

    def __asym_scc_una (self, a, k) :
        aim1 = self.phi.var ((a, 0))
#        db (aim1, 'is', (a, 0))
        for i in xrange (1, k) :
            ai = self.phi.var ((a, i))
            self.phi.add ([-ai, aim1])
#            db (ai, 'is', (a, i))
#            db ('clause', [-ai, aim1])
            aim1 = ai

    def __asym_scc_una_ord (self, a, b, k) :
        # a > b
        if self.__is_stubborn (a) :
            l = [-self.phi.var (e) for e in self.__pred (a)]
        else :
            l = [-self.phi.var (a)]

        if self.__is_stubborn (b) :
            l += [-self.phi.var (e) for e in self.__pred (b)]
        else :
            l += [-self.phi.var (b)]
        c = self.phi.var ((a, b))
        l.append (c)
        self.phi.add (l)

        ai = self.phi.var ((a, 0))
        bi = self.phi.var ((b, k - 1))
        self.phi.add ([-c, ai])
        self.phi.add ([-c, -bi])
#        db (ai, 'is', (a, 0))
#        db (bi, 'is', (b, k-1))
#        db ('clause', [-c, ai])
#        db ('clause', [-c, -bi])
        for i in xrange (k - 1) :
            bi = self.phi.var ((b, i))
            ai = self.phi.var ((a, i + 1))
            self.phi.add ([-c, -bi, ai])
#            db (bi, 'is', (b, i))
#            db (ai, 'is', (a, i+1))
#            db ('clause', [-c, -bi, ai])

    def __mark_dis (self, c) :
        if c.pre and sgl (c.pre).isblack : return
        l = [self.phi.var (c.label)]
        if c.pre :
            if self.__is_stubborn (sgl (c.pre)) :
                for e in self.__pred (sgl (c.pre)) :
                    l.append (- self.phi.var (e))
            else :
                l.append (- self.phi.var (sgl (c.pre)))
        l2 = [e for e in c.post if not e.isblack]
        if len (l2) == 1 and self.__is_stubborn (l2[0]) :
            for e in self.__pred (l2[0]) :
                self.phi.add (l + [self.phi.var (e)])
            return
        for e in l2 : l.append (self.phi.var (e))
        self.phi.add (l)

    def __dis (self) :
        tmp = set ()
        minimal = []
        tseen = set ()
        for e in self.u.events :
            if e.label in tseen : continue
            tseen.add (e.label)
            new = frozenset (c.label for c in e.pre | e.cont)
            if self.opt['sub'] :
                tmp.clear ()
                for elem in minimal :
#                    if new >= elem : db ('discarding', new);
                    if new >= elem : new = None; break
                    if new < elem : tmp.add (elem)
                for elem in tmp : minimal.remove (elem)
#                for elem in tmp : db ('removing', elem)
            if new : minimal.append (new)
#            if new : db ('taking', new)
        tmp.clear ()

        pneed = set ()
        for elem in minimal :
            self.phi.add ([-self.phi.var (p) for p in elem])
            pneed |= elem

        for c in self.u.conds :
            if c.label in pneed :
                self.__mark_dis (c)

    def __mark_cover (self, c) :
        assert (not c.pre or not sgl (c.pre).isblack)
        atm = self.phi.var (c)
        if c.pre :
            self.phi.add ([-atm, self.phi.var (sgl (c.pre))])
        for e in c.post :
            if e.isblack : continue
            self.phi.add ([-atm, -self.phi.var (e)])

    def __cover (self) :
        h = {}
        for p in self.opt['places'] : h[p] = []
        for c in self.u.conds :
            if c.label in h :
                if c.pre and sgl (c.pre).isblack : continue
                h[c.label].append (c)
                self.__mark_cover (c)
        for l in h.values () :
            self.phi.add ([self.phi.var (c) for c in l])

    def __find_places (self, l) :
        h = {}
        for p in l : h[p] = []
        for p in self.u.net.places :
            if p.name in h : h[p.name].append (p)
        for p in h :
            if len (h[p]) == 0 :
                raise Exception, "Place '%s' not found" % p
            if len (h[p]) > 1 :
                raise Exception, "There is %d places named '%s', you need to change the names in the net and unfold again!" \
                        % (len (h[p]), p)
        return [ll[0] for ll in h.values ()]

    def __reduce (self, g) :
        again = True
        while again :
            again = False
            for v in list (g) :
                i = g.in_degree (v)
                o = g.out_degree (v)
                if i == 1 :
                    used = self.__red_1 (g, v)
                    if not used : used = self.__red_5 (g, v)
                    if not used : used = self.__red_7 (g, v)
                    if not used : used = self.__red_9 (g, v)
                elif o == 1 :
                    used = self.__red_6 (g, v)
                elif i == 2 and o == 2 :
                    used = self.__red_8 (g, v)
                else :
                    used = False
                again = again or used
        return g

    def __red_edge (self, g, u, v, c) :
        g.add_edge (u, v)
        if not 'color' in g[u][v] or g[u][v]['color'] == 2 :
            g[u][v]['color'] = c

    def __red_1 (self, g, v) :
        # assume that indeg (v) == 1
        # check that pre(v) and post(v) is green

        u = g.predecessors (v)[0]
        if g[u][v]['color'] != 1 : return False
        for w in g[v] :
            if g[v][w]['color'] != 1 : return False
        for w in g[v] : g.add_edge (u, w, color=1)
        g.remove_node (v)
        return True

    def __red_5 (self, g, v) :
        # assume that indeg (v) == 1
        # check that pre(v) is red and post(v) is green

        u = g.predecessors (v)[0]
        if g[u][v]['color'] != 2 : return False
        for w in g[v] :
            if g[v][w]['color'] != 1 : return False
        for w in g[v] : self.__red_edge (g, u, w, 2)
        g.remove_node (v)
        return True

    def __red_6 (self, g, v) :
        # assume that outdeg (v) == 1
        # check that post(v) is green

        u = g.successors (v)[0]
        if g[v][u]['color'] != 1 : return False
        for w in g.predecessors (v) :
            self.__red_edge (g, w, u, g[w][v]['color'])
        g.remove_node (v)
        return True

    def __red_7 (self, g, v) :
        # assume that indeg (v) == 1 and that __red_1 is not applicable
        # check that pre(v) is green

        u = g.predecessors (v)[0]
        if g[u][v]['color'] != 1 : return False
        found = False
        l = []
        for w in g[v] :
            if g[v][w]['color'] == 1 :
                l.append (w)
                g.add_edge (u, w, color=1)
        for w in l : g.remove_edge (v, w)
        return len (l) != 0

    def __red_8 (self, g, v) :
        # assume that indeg (v) == 2 and outdeg (v) == 2
        # check that there is 1 green, 1 red incoming and 2 green outgoing

        u1 = g.predecessors (v)[0]
        u2 = g.predecessors (v)[1]
        c = 2 if g[u1][v]['color'] == 1 else 1
        if g[u2][v]['color'] != c : return False
        w1 = g.successors (v)[0]
        w2 = g.successors (v)[1]
        if g[v][w1]['color'] != 1 or g[v][w2]['color'] != 1 : return False

        self.__red_edge (g, u1, w1, g[u1][v]['color'])
        self.__red_edge (g, u1, w2, g[u1][v]['color'])
        self.__red_edge (g, u2, w1, c)
        self.__red_edge (g, u2, w2, c)
        g.remove_node (v)
        return True

    def __red_9 (self, g, v) :
        # assume that indeg (v) == 1 and that __red_5 is not applicable
        # check that pre(v) is red

        u = g.predecessors (v)[0]
        if g[u][v]['color'] != 2 : return False
        l = []
        for w in g[v] :
            if g[v][w]['color'] == 1 :
                l.append (w)
                self.__red_edge (g, u, w, 2)
        for w in l : g.remove_edge (v, w)
        return len (l) != 0

    def __is_stubborn (self, e) :
#       st3: post (pre (e) U cont (e)) without blacks subset of {e}
        if not self.opt['stb'] : return False
        if e in self.stubborn : return self.stubborn[e]

        b = True
        for c in e.pre | e.cont :
            for ep in c.post :
                if not ep.isblack and ep != e :
                    b = False
                    break

        self.stubborn[e] = b
        return b

    def __pred (self, e) :
        if e in self.pred : return self.pred[e]
        s = set ();
        for c in e.pre | e.cont :
            if not c.pre : continue
            if self.__is_stubborn (sgl (c.pre)) :
                s |= self.__pred (sgl (c.pre))
            else :
                s.add (sgl (c.pre))
        self.pred[e] = s
        return self.pred[e]

    def __runit (self, args, timeout=-1, sh=False) :
    #    db (args, timeout)
        try :
            p = subprocess.Popen (args, bufsize=8192, stdin=subprocess.PIPE,
                    stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                    preexec_fn=os.setsid, shell=sh)
        except Exception :
            raise Exception, "Unable to execute the command `minisat', is minisat installed?"

    #    db ('pid', p.pid)
        try :
            killed = False
            s = ''
            p.stdin.close ()
            if timeout > 0 :
                tref = time.time ()
                while True :
                    t = timeout - (time.time () - tref)
                    if t <= 0 : t = 0
    #                db ('select at', time.time () - tref, t)
                    (r, w, x) = select.select ([p.stdout], [], [p.stdout], t)
    #                db ('return at', time.time () - tref, r, w, x)
                    if len (r) :
                        # read (n) waits for n bytes before returning
                        c = p.stdout.read (1)
                        if len (c) == 0 : break
                        s += c
                    else :
    #                    db ('killing', p.pid)
                        os.killpg (p.pid, signal.SIGTERM)
                        killed = True
                        break
            p.wait ()
            s += p.stdout.read ()
            return (p.returncode if not killed else 254, s)
        except KeyboardInterrupt :
            os.killpg (p.pid, signal.SIGKILL)
            p.wait ()
            raise

    def solve (self) :
        pathin, pathout = '', ''
        try :
            # make temporary files for the formula and minisat's output
            fdin, pathin = tempfile.mkstemp (suffix='cna')
            fdout, pathout = tempfile.mkstemp (suffix='cna')
            # write the formula in dimacs format
            fin = os.fdopen (fdin, 'w')
            fout = os.fdopen (fdout, 'r')
            #db ('write cnf')
            self.phi.write (fin)
            fin.close ()
            # call the solver
            #db ('solve')
            s, out = self.__runit (['minisat', pathin, pathout])
            # load minisat's results
            out = fout.readlines ()
            fout.close ()
        finally :
            # remove temporary files
            os.unlink (pathin)
            os.unlink (pathout)

        places = ' '.join (["'%s'" % p.name for p in self.opt['places']])

        #db ('parse result')
        # unsatisfiable
        if s == 20 :
            r = 'NO '
            if self.opt['deadlock'] :
                r += ', the net is deadlock-free'
            if self.opt['coverability'] :
                r += ', no rechable marking covers %s' % places
            self.result['answer'] = r
            return

        # errors
        if s != 10 :
            raise Exception, \
                    'Minisat terminated with unknown exit status %d' % s

        # satisfiable
        r = 'YES'
        if self.opt['deadlock'] :
            r += ', a deadlocked marking is reachable'
        if self.opt['coverability'] :
            if len (self.opt['places']) > 1 :
                s = 's'
                are = 'are'
            else :
                s = ''
                are = 'is'
            r += ', place%s %s %s coverable' % (s, places, are)
        self.result['answer'] = r

        # build the trace
        #db ('trace', out)
        if len (out) != 2 :
            raise Exception, 'Error while parsing minisat output'
        out = out[1].split ()
        try :
            out = [int (x) for x in out]
        except :
            raise Exception, 'Error when parsing minisat output'
        out = set ([x for x in out if x > 0])
        trace = set ()
        for e in self.u.events :
            if self.phi.var (e) in out : trace |= self.u.cone (e)
        try :
            trace = ["'%s'" % repr (e) for e in self.u.run_of (trace)]
        except :
            trace = ["'%s'" % repr (e) for e in trace]
            self.result['note'] = \
'Looks like the trace contains a cycle of asymmetric conflict. Did you use \
the "nocy" reduction?'
        if len (trace) == 0 and self.u.nr_black == 0 :
            k = 'note' if 'note' not in self.result else 'note1'
            self.result[k] = \
'The unfolding has no cutoff, all executions are finite!'
        self.result['trace'] = ' '.join (trace)

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

def parse () :
    p = argparse.ArgumentParser (usage = __doc__)
    g = p.add_mutually_exclusive_group (required=True)
    p.add_argument ('-v', '--verbose', action = 'store_true')
    g.add_argument ('-d', '--deadlock', action = 'store_true')
    g.add_argument ('-c', '--cover', nargs = '+')
    p.add_argument ('-n', '--dont-solve')
    p.add_argument ('-r', '--reduce',
        nargs = '+',
        choices = [ '2-tree', '3-tree', '4-tree', '5-tree', '6-tree', '7-tree',
                '8-tree', '9-tree', 'seq', 'log', 'pair', 'stb', 'sub', 'nocy',
                'bin', 'unary', 'trans', 'sccred'])

    p.add_argument ('infile',
        nargs = '?',
        type = lambda x : opendef (p, x, 'r'),
        default = '-')

    p.add_argument ('outfile',
        nargs = '?',
        type = lambda x : opendef (p, x, 'w'),
        default = '-')

    args = p.parse_args ()
    if args.dont_solve != None :
        args.dont_solve = opendef (p, args.dont_solve, 'w')
    return args

def output (f, k, v, n, fmt='%s') :
    f.write (('%-*s : ' + fmt + '\n') % (n, k, v))


def usage () :
    print __doc__
    sys.exit (0);

def main () :
    if (len (sys.argv) != 3) : usage ()
    net_path = sys.argv[1]
    formula_path = sys.argv[2]

    print sys.argv

    n = ptnet.Net (True)
    try :
        f = open (net_path, 'r')
        n.read (f, fmt='pnml')
        f.close ()
    except IOError as (e, m) :
        raise Exception, "'%s': %s" % (net_path, m)

    # test markings, iterators, indexes
    # test enables, enabled
    for t in n.trans :
        print t

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
    d[m1] = 999999999;
    d[m2] = 999999999;
    for k,v in d.items () :
        print k
        print v

    print 'ccccccccccccccccccccc'
    g = networkx.DiGraph ()
    g.add_edge (m1, m2)
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

if __name__ == '__main__' :
    main ()
    try :
        #main ()
        print ""
    except KeyboardInterrupt :
        print 'cna: interrupted'
        sys.exit (1)
    except Exception as e :
        print 'cna: error: %s' % str (e)
        sys.exit (1)
    sys.exit (0)

# vi:ts=4:sw=4:et:
