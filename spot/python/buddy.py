# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.10
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_buddy')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_buddy')
    _buddy = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_buddy', [dirname(__file__)])
        except ImportError:
            import _buddy
            return _buddy
        if fp is not None:
            try:
                _mod = imp.load_module('_buddy', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _buddy = swig_import_helper()
    del swig_import_helper
else:
    import _buddy
del _swig_python_version_info
try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

def _swig_setattr_nondynamic(self, class_type, name, value, static=1):
    if (name == "thisown"):
        return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name, None)
    if method:
        return method(self, value)
    if (not static):
        object.__setattr__(self, name, value)
    else:
        raise AttributeError("You cannot add attributes to %s" % self)


def _swig_setattr(self, class_type, name, value):
    return _swig_setattr_nondynamic(self, class_type, name, value, 0)


def _swig_getattr(self, class_type, name):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    raise AttributeError("'%s' object has no attribute '%s'" % (class_type.__name__, name))


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except __builtin__.Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)


def _swig_setattr_nondynamic_method(set):
    def set_attr(self, name, value):
        if (name == "thisown"):
            return self.this.own(value)
        if hasattr(self, name) or (name == "this"):
            set(self, name, value)
        else:
            raise AttributeError("You cannot add attributes to %s" % self)
    return set_attr


class const_int_ptr(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, ptr: 'int const *'):
        _buddy.const_int_ptr_swiginit(self, _buddy.new_const_int_ptr(ptr))
    ptr = _swig_property(_buddy.const_int_ptr_ptr_get, _buddy.const_int_ptr_ptr_set)

    def __getitem__(self, i: 'int') -> "int":
        return _buddy.const_int_ptr___getitem__(self, i)
    __swig_destroy__ = _buddy.delete_const_int_ptr
const_int_ptr_swigregister = _buddy.const_int_ptr_swigregister
const_int_ptr_swigregister(const_int_ptr)

class bdd(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def id(self) -> "int":
        return _buddy.bdd_id(self)

    def __cmp__(self, b: 'bdd') -> "int":
        return _buddy.bdd___cmp__(self, b)

    def __le__(self, b: 'bdd') -> "bool":
        return _buddy.bdd___le__(self, b)

    def __lt__(self, b: 'bdd') -> "bool":
        return _buddy.bdd___lt__(self, b)

    def __eq__(self, b: 'bdd') -> "bool":
        return _buddy.bdd___eq__(self, b)

    def __ne__(self, b: 'bdd') -> "bool":
        return _buddy.bdd___ne__(self, b)

    def __ge__(self, b: 'bdd') -> "bool":
        return _buddy.bdd___ge__(self, b)

    def __gt__(self, b: 'bdd') -> "bool":
        return _buddy.bdd___gt__(self, b)

    def __hash__(self) -> "size_t":
        return _buddy.bdd___hash__(self)

    def __str__(self) -> "std::string":
        return _buddy.bdd___str__(self)

    def __and__(self, other: 'bdd') -> "bdd":
        return _buddy.bdd___and__(self, other)

    def __xor__(self, other: 'bdd') -> "bdd":
        return _buddy.bdd___xor__(self, other)

    def __or__(self, other: 'bdd') -> "bdd":
        return _buddy.bdd___or__(self, other)

    def __rshift__(self, other: 'bdd') -> "bdd":
        return _buddy.bdd___rshift__(self, other)

    def __lshift__(self, other: 'bdd') -> "bdd":
        return _buddy.bdd___lshift__(self, other)

    def __sub__(self, other: 'bdd') -> "bdd":
        return _buddy.bdd___sub__(self, other)

    def __neg__(self) -> "bdd":
        return _buddy.bdd___neg__(self)

    def __init__(self):
        _buddy.bdd_swiginit(self, _buddy.new_bdd())
    __swig_destroy__ = _buddy.delete_bdd
bdd_swigregister = _buddy.bdd_swigregister
bdd_swigregister(bdd)


def bdd_init(arg1: 'int', arg2: 'int') -> "int":
    return _buddy.bdd_init(arg1, arg2)
bdd_init = _buddy.bdd_init

def bdd_done() -> "void":
    return _buddy.bdd_done()
bdd_done = _buddy.bdd_done

def bdd_setvarnum(arg1: 'int') -> "int":
    return _buddy.bdd_setvarnum(arg1)
bdd_setvarnum = _buddy.bdd_setvarnum

def bdd_extvarnum(arg1: 'int') -> "int":
    return _buddy.bdd_extvarnum(arg1)
bdd_extvarnum = _buddy.bdd_extvarnum

def bdd_isrunning() -> "int":
    return _buddy.bdd_isrunning()
bdd_isrunning = _buddy.bdd_isrunning

def bdd_setmaxnodenum(arg1: 'int') -> "int":
    return _buddy.bdd_setmaxnodenum(arg1)
bdd_setmaxnodenum = _buddy.bdd_setmaxnodenum

def bdd_setmaxincrease(arg1: 'int') -> "int":
    return _buddy.bdd_setmaxincrease(arg1)
bdd_setmaxincrease = _buddy.bdd_setmaxincrease

def bdd_setminfreenodes(arg1: 'int') -> "int":
    return _buddy.bdd_setminfreenodes(arg1)
bdd_setminfreenodes = _buddy.bdd_setminfreenodes

def bdd_getnodenum() -> "int":
    return _buddy.bdd_getnodenum()
bdd_getnodenum = _buddy.bdd_getnodenum

def bdd_getallocnum() -> "int":
    return _buddy.bdd_getallocnum()
bdd_getallocnum = _buddy.bdd_getallocnum

def bdd_versionstr() -> "char *":
    return _buddy.bdd_versionstr()
bdd_versionstr = _buddy.bdd_versionstr

def bdd_versionnum() -> "int":
    return _buddy.bdd_versionnum()
bdd_versionnum = _buddy.bdd_versionnum

def bdd_fprintstat(arg1: 'FILE *') -> "void":
    return _buddy.bdd_fprintstat(arg1)
bdd_fprintstat = _buddy.bdd_fprintstat

def bdd_printstat() -> "void":
    return _buddy.bdd_printstat()
bdd_printstat = _buddy.bdd_printstat

def bdd_errstring(arg1: 'int') -> "char const *":
    return _buddy.bdd_errstring(arg1)
bdd_errstring = _buddy.bdd_errstring

def bdd_clear_error() -> "void":
    return _buddy.bdd_clear_error()
bdd_clear_error = _buddy.bdd_clear_error

def bdd_ithvar(v: 'int') -> "bdd":
    return _buddy.bdd_ithvar(v)
bdd_ithvar = _buddy.bdd_ithvar

def bdd_nithvar(v: 'int') -> "bdd":
    return _buddy.bdd_nithvar(v)
bdd_nithvar = _buddy.bdd_nithvar

def bdd_var(r: 'bdd') -> "int":
    return _buddy.bdd_var(r)
bdd_var = _buddy.bdd_var

def bdd_low(r: 'bdd') -> "bdd":
    return _buddy.bdd_low(r)
bdd_low = _buddy.bdd_low

def bdd_high(r: 'bdd') -> "bdd":
    return _buddy.bdd_high(r)
bdd_high = _buddy.bdd_high

def bdd_scanset(r: 'bdd', v: 'int *&', n: 'int &') -> "int":
    return _buddy.bdd_scanset(r, v, n)
bdd_scanset = _buddy.bdd_scanset

def bdd_makeset(v: 'int *', n: 'int') -> "bdd":
    return _buddy.bdd_makeset(v, n)
bdd_makeset = _buddy.bdd_makeset

def bdd_setbddpair(p: 'bddPair *', ov: 'int', nv: 'bdd') -> "int":
    return _buddy.bdd_setbddpair(p, ov, nv)
bdd_setbddpair = _buddy.bdd_setbddpair

def bdd_replace(r: 'bdd', p: 'bddPair *') -> "bdd":
    return _buddy.bdd_replace(r, p)
bdd_replace = _buddy.bdd_replace

def bdd_compose(f: 'bdd', g: 'bdd', v: 'int') -> "bdd":
    return _buddy.bdd_compose(f, g, v)
bdd_compose = _buddy.bdd_compose

def bdd_veccompose(f: 'bdd', p: 'bddPair *') -> "bdd":
    return _buddy.bdd_veccompose(f, p)
bdd_veccompose = _buddy.bdd_veccompose

def bdd_restrict(r: 'bdd', var: 'bdd') -> "bdd":
    return _buddy.bdd_restrict(r, var)
bdd_restrict = _buddy.bdd_restrict

def bdd_constrain(f: 'bdd', c: 'bdd') -> "bdd":
    return _buddy.bdd_constrain(f, c)
bdd_constrain = _buddy.bdd_constrain

def bdd_simplify(d: 'bdd', b: 'bdd') -> "bdd":
    return _buddy.bdd_simplify(d, b)
bdd_simplify = _buddy.bdd_simplify

def bdd_ibuildcube(v: 'int', w: 'int', a: 'int *') -> "bdd":
    return _buddy.bdd_ibuildcube(v, w, a)
bdd_ibuildcube = _buddy.bdd_ibuildcube

def bdd_not(r: 'bdd') -> "bdd":
    return _buddy.bdd_not(r)
bdd_not = _buddy.bdd_not

def bdd_apply(l: 'bdd', r: 'bdd', op: 'int') -> "bdd":
    return _buddy.bdd_apply(l, r, op)
bdd_apply = _buddy.bdd_apply

def bdd_and(l: 'bdd', r: 'bdd') -> "bdd":
    return _buddy.bdd_and(l, r)
bdd_and = _buddy.bdd_and

def bdd_or(l: 'bdd', r: 'bdd') -> "bdd":
    return _buddy.bdd_or(l, r)
bdd_or = _buddy.bdd_or

def bdd_xor(l: 'bdd', r: 'bdd') -> "bdd":
    return _buddy.bdd_xor(l, r)
bdd_xor = _buddy.bdd_xor

def bdd_imp(l: 'bdd', r: 'bdd') -> "bdd":
    return _buddy.bdd_imp(l, r)
bdd_imp = _buddy.bdd_imp

def bdd_biimp(l: 'bdd', r: 'bdd') -> "bdd":
    return _buddy.bdd_biimp(l, r)
bdd_biimp = _buddy.bdd_biimp

def bdd_setxor(l: 'bdd', r: 'bdd') -> "bdd":
    return _buddy.bdd_setxor(l, r)
bdd_setxor = _buddy.bdd_setxor

def bdd_implies(l: 'bdd', r: 'bdd') -> "int":
    return _buddy.bdd_implies(l, r)
bdd_implies = _buddy.bdd_implies

def bdd_ite(f: 'bdd', g: 'bdd', h: 'bdd') -> "bdd":
    return _buddy.bdd_ite(f, g, h)
bdd_ite = _buddy.bdd_ite

def bdd_exist(r: 'bdd', var: 'bdd') -> "bdd":
    return _buddy.bdd_exist(r, var)
bdd_exist = _buddy.bdd_exist

def bdd_existcomp(r: 'bdd', var: 'bdd') -> "bdd":
    return _buddy.bdd_existcomp(r, var)
bdd_existcomp = _buddy.bdd_existcomp

def bdd_forall(r: 'bdd', var: 'bdd') -> "bdd":
    return _buddy.bdd_forall(r, var)
bdd_forall = _buddy.bdd_forall

def bdd_forallcomp(r: 'bdd', var: 'bdd') -> "bdd":
    return _buddy.bdd_forallcomp(r, var)
bdd_forallcomp = _buddy.bdd_forallcomp

def bdd_unique(r: 'bdd', var: 'bdd') -> "bdd":
    return _buddy.bdd_unique(r, var)
bdd_unique = _buddy.bdd_unique

def bdd_uniquecomp(r: 'bdd', var: 'bdd') -> "bdd":
    return _buddy.bdd_uniquecomp(r, var)
bdd_uniquecomp = _buddy.bdd_uniquecomp

def bdd_appex(l: 'bdd', r: 'bdd', op: 'int', var: 'bdd') -> "bdd":
    return _buddy.bdd_appex(l, r, op, var)
bdd_appex = _buddy.bdd_appex

def bdd_appexcomp(l: 'bdd', r: 'bdd', op: 'int', var: 'bdd') -> "bdd":
    return _buddy.bdd_appexcomp(l, r, op, var)
bdd_appexcomp = _buddy.bdd_appexcomp

def bdd_appall(l: 'bdd', r: 'bdd', op: 'int', var: 'bdd') -> "bdd":
    return _buddy.bdd_appall(l, r, op, var)
bdd_appall = _buddy.bdd_appall

def bdd_appallcomp(l: 'bdd', r: 'bdd', op: 'int', var: 'bdd') -> "bdd":
    return _buddy.bdd_appallcomp(l, r, op, var)
bdd_appallcomp = _buddy.bdd_appallcomp

def bdd_appuni(l: 'bdd', r: 'bdd', op: 'int', var: 'bdd') -> "bdd":
    return _buddy.bdd_appuni(l, r, op, var)
bdd_appuni = _buddy.bdd_appuni

def bdd_appunicomp(l: 'bdd', r: 'bdd', op: 'int', var: 'bdd') -> "bdd":
    return _buddy.bdd_appunicomp(l, r, op, var)
bdd_appunicomp = _buddy.bdd_appunicomp

def bdd_support(r: 'bdd') -> "bdd":
    return _buddy.bdd_support(r)
bdd_support = _buddy.bdd_support

def bdd_satone(r: 'bdd') -> "bdd":
    return _buddy.bdd_satone(r)
bdd_satone = _buddy.bdd_satone

def bdd_satoneset(r: 'bdd', var: 'bdd', pol: 'bdd') -> "bdd":
    return _buddy.bdd_satoneset(r, var, pol)
bdd_satoneset = _buddy.bdd_satoneset

def bdd_fullsatone(r: 'bdd') -> "bdd":
    return _buddy.bdd_fullsatone(r)
bdd_fullsatone = _buddy.bdd_fullsatone

def bdd_allsat(r: 'bdd', handler: 'bddallsathandler') -> "void":
    return _buddy.bdd_allsat(r, handler)
bdd_allsat = _buddy.bdd_allsat

def bdd_satcount(r: 'bdd') -> "double":
    return _buddy.bdd_satcount(r)
bdd_satcount = _buddy.bdd_satcount

def bdd_satcountset(r: 'bdd', varset: 'bdd') -> "double":
    return _buddy.bdd_satcountset(r, varset)
bdd_satcountset = _buddy.bdd_satcountset

def bdd_satcountln(r: 'bdd') -> "double":
    return _buddy.bdd_satcountln(r)
bdd_satcountln = _buddy.bdd_satcountln

def bdd_satcountlnset(r: 'bdd', varset: 'bdd') -> "double":
    return _buddy.bdd_satcountlnset(r, varset)
bdd_satcountlnset = _buddy.bdd_satcountlnset

def bdd_nodecount(r: 'bdd') -> "int":
    return _buddy.bdd_nodecount(r)
bdd_nodecount = _buddy.bdd_nodecount

def bdd_varprofile(r: 'bdd') -> "int *":
    return _buddy.bdd_varprofile(r)
bdd_varprofile = _buddy.bdd_varprofile

def bdd_pathcount(r: 'bdd') -> "double":
    return _buddy.bdd_pathcount(r)
bdd_pathcount = _buddy.bdd_pathcount

def bdd_fprinttable(file: 'FILE *', r: 'bdd') -> "void":
    return _buddy.bdd_fprinttable(file, r)
bdd_fprinttable = _buddy.bdd_fprinttable

def bdd_printtable(r: 'bdd') -> "void":
    return _buddy.bdd_printtable(r)
bdd_printtable = _buddy.bdd_printtable

def bdd_fprintset(file: 'FILE *', r: 'bdd') -> "void":
    return _buddy.bdd_fprintset(file, r)
bdd_fprintset = _buddy.bdd_fprintset

def bdd_printset(r: 'bdd') -> "void":
    return _buddy.bdd_printset(r)
bdd_printset = _buddy.bdd_printset

def bdd_printdot(r: 'bdd') -> "void":
    return _buddy.bdd_printdot(r)
bdd_printdot = _buddy.bdd_printdot

def bdd_fprintdot(ofile: 'FILE *', r: 'bdd') -> "void":
    return _buddy.bdd_fprintdot(ofile, r)
bdd_fprintdot = _buddy.bdd_fprintdot

def bdd_fnprintdot(fname: 'char *', r: 'bdd') -> "int":
    return _buddy.bdd_fnprintdot(fname, r)
bdd_fnprintdot = _buddy.bdd_fnprintdot

def bdd_fnsave(fname: 'char *', r: 'bdd') -> "int":
    return _buddy.bdd_fnsave(fname, r)
bdd_fnsave = _buddy.bdd_fnsave

def bdd_save(ofile: 'FILE *', r: 'bdd') -> "int":
    return _buddy.bdd_save(ofile, r)
bdd_save = _buddy.bdd_save

def bdd_fnload(fname: 'char *', r: 'bdd') -> "int":
    return _buddy.bdd_fnload(fname, r)
bdd_fnload = _buddy.bdd_fnload

def bdd_load(ifile: 'FILE *', r: 'bdd') -> "int":
    return _buddy.bdd_load(ifile, r)
bdd_load = _buddy.bdd_load

def bdd_addvarblock(v: 'bdd', f: 'int') -> "int":
    return _buddy.bdd_addvarblock(v, f)
bdd_addvarblock = _buddy.bdd_addvarblock
bddop_and = _buddy.bddop_and
bddop_xor = _buddy.bddop_xor
bddop_or = _buddy.bddop_or
bddop_nand = _buddy.bddop_nand
bddop_nor = _buddy.bddop_nor
bddop_imp = _buddy.bddop_imp
bddop_biimp = _buddy.bddop_biimp
bddop_diff = _buddy.bddop_diff
bddop_less = _buddy.bddop_less
bddop_invimp = _buddy.bddop_invimp
BDD_REORDER_NONE = _buddy.BDD_REORDER_NONE
BDD_REORDER_WIN2 = _buddy.BDD_REORDER_WIN2
BDD_REORDER_WIN2ITE = _buddy.BDD_REORDER_WIN2ITE
BDD_REORDER_SIFT = _buddy.BDD_REORDER_SIFT
BDD_REORDER_SIFTITE = _buddy.BDD_REORDER_SIFTITE
BDD_REORDER_WIN3 = _buddy.BDD_REORDER_WIN3
BDD_REORDER_WIN3ITE = _buddy.BDD_REORDER_WIN3ITE
BDD_REORDER_RANDOM = _buddy.BDD_REORDER_RANDOM

def fdd_extdomain(input_buf: 'int *') -> "int":
    return _buddy.fdd_extdomain(input_buf)
fdd_extdomain = _buddy.fdd_extdomain

def fdd_overlapdomain(arg1: 'int', arg2: 'int') -> "int":
    return _buddy.fdd_overlapdomain(arg1, arg2)
fdd_overlapdomain = _buddy.fdd_overlapdomain

def fdd_clearall() -> "void":
    return _buddy.fdd_clearall()
fdd_clearall = _buddy.fdd_clearall

def fdd_domainnum() -> "int":
    return _buddy.fdd_domainnum()
fdd_domainnum = _buddy.fdd_domainnum

def fdd_domainsize(arg1: 'int') -> "int":
    return _buddy.fdd_domainsize(arg1)
fdd_domainsize = _buddy.fdd_domainsize

def fdd_varnum(arg1: 'int') -> "int":
    return _buddy.fdd_varnum(arg1)
fdd_varnum = _buddy.fdd_varnum

def fdd_vars(arg1: 'int') -> "const_int_ptr":
    return _buddy.fdd_vars(arg1)
fdd_vars = _buddy.fdd_vars

def fdd_ithvar(arg1: 'int', arg2: 'int') -> "bdd":
    return _buddy.fdd_ithvar(arg1, arg2)
fdd_ithvar = _buddy.fdd_ithvar

def fdd_scanvar(arg1: 'bdd', arg2: 'int') -> "int":
    return _buddy.fdd_scanvar(arg1, arg2)
fdd_scanvar = _buddy.fdd_scanvar

def fdd_scanallvar(arg1: 'bdd') -> "int *":
    return _buddy.fdd_scanallvar(arg1)
fdd_scanallvar = _buddy.fdd_scanallvar

def fdd_ithset(arg1: 'int') -> "bdd":
    return _buddy.fdd_ithset(arg1)
fdd_ithset = _buddy.fdd_ithset

def fdd_domain(arg1: 'int') -> "bdd":
    return _buddy.fdd_domain(arg1)
fdd_domain = _buddy.fdd_domain

def fdd_equals(arg1: 'int', arg2: 'int') -> "bdd":
    return _buddy.fdd_equals(arg1, arg2)
fdd_equals = _buddy.fdd_equals

def fdd_printset(arg1: 'bdd') -> "void":
    return _buddy.fdd_printset(arg1)
fdd_printset = _buddy.fdd_printset

def fdd_fprintset(arg1: 'FILE *', arg2: 'bdd') -> "void":
    return _buddy.fdd_fprintset(arg1, arg2)
fdd_fprintset = _buddy.fdd_fprintset

def fdd_scanset(arg1: 'bdd', arg2: 'int *&', arg3: 'int &') -> "int":
    return _buddy.fdd_scanset(arg1, arg2, arg3)
fdd_scanset = _buddy.fdd_scanset

def fdd_makeset(arg1: 'int *', arg2: 'int') -> "bdd":
    return _buddy.fdd_makeset(arg1, arg2)
fdd_makeset = _buddy.fdd_makeset

def fdd_intaddvarblock(arg1: 'int', arg2: 'int', arg3: 'int') -> "int":
    return _buddy.fdd_intaddvarblock(arg1, arg2, arg3)
fdd_intaddvarblock = _buddy.fdd_intaddvarblock

def fdd_setpair(arg1: 'bddPair *', arg2: 'int', arg3: 'int') -> "int":
    return _buddy.fdd_setpair(arg1, arg2, arg3)
fdd_setpair = _buddy.fdd_setpair

def fdd_setpairs(arg1: 'bddPair *', arg2: 'int *', arg3: 'int *', arg4: 'int') -> "int":
    return _buddy.fdd_setpairs(arg1, arg2, arg3, arg4)
fdd_setpairs = _buddy.fdd_setpairs

def bvec_copy(v: 'bvec') -> "bvec":
    return _buddy.bvec_copy(v)
bvec_copy = _buddy.bvec_copy

def bvec_true(bitnum: 'int') -> "bvec":
    return _buddy.bvec_true(bitnum)
bvec_true = _buddy.bvec_true

def bvec_false(bitnum: 'int') -> "bvec":
    return _buddy.bvec_false(bitnum)
bvec_false = _buddy.bvec_false

def bvec_con(bitnum: 'int', val: 'int') -> "bvec":
    return _buddy.bvec_con(bitnum, val)
bvec_con = _buddy.bvec_con

def bvec_var(bitnum: 'int', offset: 'int', step: 'int') -> "bvec":
    return _buddy.bvec_var(bitnum, offset, step)
bvec_var = _buddy.bvec_var

def bvec_varfdd(var: 'int') -> "bvec":
    return _buddy.bvec_varfdd(var)
bvec_varfdd = _buddy.bvec_varfdd

def bvec_varvec(bitnum: 'int', var: 'int *') -> "bvec":
    return _buddy.bvec_varvec(bitnum, var)
bvec_varvec = _buddy.bvec_varvec

def bvec_coerce(bitnum: 'int', v: 'bvec') -> "bvec":
    return _buddy.bvec_coerce(bitnum, v)
bvec_coerce = _buddy.bvec_coerce

def bvec_isconst(e: 'bvec') -> "int":
    return _buddy.bvec_isconst(e)
bvec_isconst = _buddy.bvec_isconst

def bvec_val(e: 'bvec') -> "int":
    return _buddy.bvec_val(e)
bvec_val = _buddy.bvec_val

def bvec_map1(arg1: 'bvec', fun: 'bdd (*)(bdd const &)') -> "bvec":
    return _buddy.bvec_map1(arg1, fun)
bvec_map1 = _buddy.bvec_map1

def bvec_map2(arg1: 'bvec', arg2: 'bvec', fun: 'bdd (*)(bdd const &,bdd const &)') -> "bvec":
    return _buddy.bvec_map2(arg1, arg2, fun)
bvec_map2 = _buddy.bvec_map2

def bvec_map3(arg1: 'bvec', arg2: 'bvec', arg3: 'bvec', fun: 'bdd (*)(bdd const &,bdd const &,bdd const &)') -> "bvec":
    return _buddy.bvec_map3(arg1, arg2, arg3, fun)
bvec_map3 = _buddy.bvec_map3

def bvec_add(left: 'bvec', right: 'bvec') -> "bvec":
    return _buddy.bvec_add(left, right)
bvec_add = _buddy.bvec_add

def bvec_sub(left: 'bvec', right: 'bvec') -> "bvec":
    return _buddy.bvec_sub(left, right)
bvec_sub = _buddy.bvec_sub

def bvec_mulfixed(e: 'bvec', c: 'int') -> "bvec":
    return _buddy.bvec_mulfixed(e, c)
bvec_mulfixed = _buddy.bvec_mulfixed

def bvec_mul(left: 'bvec', right: 'bvec') -> "bvec":
    return _buddy.bvec_mul(left, right)
bvec_mul = _buddy.bvec_mul

def bvec_divfixed(arg1: 'bvec', c: 'int', arg3: 'bvec', arg4: 'bvec') -> "int":
    return _buddy.bvec_divfixed(arg1, c, arg3, arg4)
bvec_divfixed = _buddy.bvec_divfixed

def bvec_div(arg1: 'bvec', arg2: 'bvec', arg3: 'bvec', arg4: 'bvec') -> "int":
    return _buddy.bvec_div(arg1, arg2, arg3, arg4)
bvec_div = _buddy.bvec_div

def bvec_ite(a: 'bdd', b: 'bvec', c: 'bvec') -> "bvec":
    return _buddy.bvec_ite(a, b, c)
bvec_ite = _buddy.bvec_ite

def bvec_shlfixed(e: 'bvec', pos: 'int', c: 'bdd') -> "bvec":
    return _buddy.bvec_shlfixed(e, pos, c)
bvec_shlfixed = _buddy.bvec_shlfixed

def bvec_shl(l: 'bvec', r: 'bvec', c: 'bdd') -> "bvec":
    return _buddy.bvec_shl(l, r, c)
bvec_shl = _buddy.bvec_shl

def bvec_shrfixed(e: 'bvec', pos: 'int', c: 'bdd') -> "bvec":
    return _buddy.bvec_shrfixed(e, pos, c)
bvec_shrfixed = _buddy.bvec_shrfixed

def bvec_shr(l: 'bvec', r: 'bvec', c: 'bdd') -> "bvec":
    return _buddy.bvec_shr(l, r, c)
bvec_shr = _buddy.bvec_shr

def bvec_lth(left: 'bvec', right: 'bvec') -> "bdd":
    return _buddy.bvec_lth(left, right)
bvec_lth = _buddy.bvec_lth

def bvec_lte(left: 'bvec', right: 'bvec') -> "bdd":
    return _buddy.bvec_lte(left, right)
bvec_lte = _buddy.bvec_lte

def bvec_gth(left: 'bvec', right: 'bvec') -> "bdd":
    return _buddy.bvec_gth(left, right)
bvec_gth = _buddy.bvec_gth

def bvec_gte(left: 'bvec', right: 'bvec') -> "bdd":
    return _buddy.bvec_gte(left, right)
bvec_gte = _buddy.bvec_gte

def bvec_equ(left: 'bvec', right: 'bvec') -> "bdd":
    return _buddy.bvec_equ(left, right)
bvec_equ = _buddy.bvec_equ

def bvec_neq(left: 'bvec', right: 'bvec') -> "bdd":
    return _buddy.bvec_neq(left, right)
bvec_neq = _buddy.bvec_neq
class bvec(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        _buddy.bvec_swiginit(self, _buddy.new_bvec(*args))
    __swig_destroy__ = _buddy.delete_bvec

    def set(self, i: 'int', b: 'bdd') -> "void":
        return _buddy.bvec_set(self, i, b)

    def bitnum(self) -> "int":
        return _buddy.bvec_bitnum(self)

    def empty(self) -> "int":
        return _buddy.bvec_empty(self)

    def __and__(self, a: 'bvec') -> "bvec":
        return _buddy.bvec___and__(self, a)

    def __xor__(self, a: 'bvec') -> "bvec":
        return _buddy.bvec___xor__(self, a)

    def __or__(self, a: 'bvec') -> "bvec":
        return _buddy.bvec___or__(self, a)

    def __lshift__(self, *args) -> "bvec":
        return _buddy.bvec___lshift__(self, *args)

    def __rshift__(self, *args) -> "bvec":
        return _buddy.bvec___rshift__(self, *args)

    def __add__(self, a: 'bvec') -> "bvec":
        return _buddy.bvec___add__(self, a)

    def __sub__(self, a: 'bvec') -> "bvec":
        return _buddy.bvec___sub__(self, a)

    def __mul__(self, *args) -> "bvec":
        return _buddy.bvec___mul__(self, *args)

    def __lt__(self, a: 'bvec') -> "bdd":
        return _buddy.bvec___lt__(self, a)

    def __le__(self, a: 'bvec') -> "bdd":
        return _buddy.bvec___le__(self, a)

    def __gt__(self, a: 'bvec') -> "bdd":
        return _buddy.bvec___gt__(self, a)

    def __ge__(self, a: 'bvec') -> "bdd":
        return _buddy.bvec___ge__(self, a)

    def __eq__(self, a: 'bvec') -> "bdd":
        return _buddy.bvec___eq__(self, a)

    def __ne__(self, a: 'bvec') -> "bdd":
        return _buddy.bvec___ne__(self, a)

    def __str__(self) -> "std::string":
        return _buddy.bvec___str__(self)

    def __getitem__(self, i: 'int') -> "bdd":
        return _buddy.bvec___getitem__(self, i)
bvec_swigregister = _buddy.bvec_swigregister
bvec_swigregister(bvec)
cvar = _buddy.cvar
bddfalse = cvar.bddfalse
bddtrue = cvar.bddtrue



