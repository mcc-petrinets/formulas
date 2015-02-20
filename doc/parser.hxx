#include <iostream>
#include <sstream>
#include <fstream>
#include <tr1/memory>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>

#include "mccproperty.hxx"

using std::cerr;
using std::endl;
using std::vector;
using std::string;
using std::tr1::shared_ptr;
using namespace boost::spirit;
using namespace boost::phoenix;

typedef shared_ptr<mcc::property_set>
        property_set;
typedef shared_ptr<mcc::property>
        property;
typedef shared_ptr<mcc::formula>
        formula;
typedef shared_ptr<mcc::place>
        place;
typedef shared_ptr<mcc::transition>
        transition;
typedef shared_ptr<mcc::level>
        level;
typedef shared_ptr<mcc::integer>
        integer;
typedef shared_ptr<mcc::multiset>
        multiset;
typedef shared_ptr<mcc::strength>
        strength;
typedef shared_ptr<mcc::property_tag>
        property_tag;
typedef shared_ptr<mcc::binding>
        binding;
typedef shared_ptr<mcc::value_variable>
        value_variable;
typedef shared_ptr<mcc::value>
        value;
typedef shared_ptr<mcc::term>
        term;

void
show(string const& s)
{
  std::cout << s << std::endl;
}

void
update_property_set(const property_set& s, const vector<property>& v)
{
  for (vector<property>::const_iterator i = v.begin(); i != v.end(); ++i)
  {
    s->property().push_back(**i);
  }
}

void
update_term(const term& t, const vector<binding>& v)
{
  if (mcc::tuple* x = dynamic_cast<mcc::tuple*>(t.get()))
  {
    for (vector<binding>::const_iterator i = v.begin(); i != v.end(); ++i)
    {
      x->binding().push_back(**i);
    }
  }
}

template <typename T, typename Derived>
void
update_name(const T& p, const string& n)
{
  if (Derived* x = dynamic_cast<Derived*>(p.get()))
  {
    x->name(n);
  }
  else
    assert(false);
}

template <typename T, typename Derived>
void
update_auto_name(const T& p, int n)
{
  std::ostringstream oss;
  oss << "_" << n;
  if (Derived* x = dynamic_cast<Derived*>(p.get()))
  {
    x->name(oss.str());
  }
  else
    assert(false);
}

template <typename T, typename Derived>
void
update_value(const T& p, const std::string& n)
{
  if (Derived* x = dynamic_cast<Derived*>(p.get()))
  {
    x->value1(n);
  }
  else
    assert(false);
}

template <typename T, typename Derived>
void
update_bindings(const T& x, const vector<binding>& v)
{
  if (Derived* y = dynamic_cast<Derived*>(x.get()))
  {
    for (typename vector<binding>::const_iterator i = v.begin(); i != v.end(); ++i)
    {
      y->binding().push_back(**i);
    }
  }
}

template <typename Derived>
void
update_composite(const formula& f, const vector<formula>& v)
{
  if (Derived* x = dynamic_cast<Derived*>(f.get()))
  {
    for (typename vector<formula>::const_iterator i = v.begin(); i != v.end(); ++i)
    {
      x->formula1().push_back(**i);
    }
  }
}

void
update_integer_constant(const integer& i, int v)
{
  if (mcc::integer_constant* x = dynamic_cast<mcc::integer_constant*>(i.get()))
  {
    x->value(v);
  }
  else
    assert(false);  
}

template <typename Derived>
void
update_complex_integer(const integer& i, const std::vector<integer>& v)
{
  if (Derived* x = dynamic_cast<Derived*>(i.get()))
  {
    for (typename vector<integer>::const_iterator j = v.begin(); j != v.end(); ++j)
    {
      x->integer1().push_back(**j);
    }
  }
}

template <typename Derived>
void
update_integer_comparison(const formula& f, const std::vector<integer>& v)
{
  if (Derived* x = dynamic_cast<Derived*>(f.get()))
  {
    for (typename vector<integer>::const_iterator j = v.begin(); j != v.end(); ++j)
    {
      x->integer().push_back(**j);
    }
  }
}

template <typename Derived>
void
update_complex_multiset(const multiset& m, const std::vector<multiset>& v)
{
  if (Derived* x = dynamic_cast<Derived*>(m.get()))
  {
    for (typename vector<multiset>::const_iterator j = v.begin(); j != v.end(); ++j)
    {
      x->multiset1().push_back(**j);
    }
  }
}

template <typename Derived>
void
update_multiset_comparison(const formula& f, const std::vector<multiset>& v)
{
  if (Derived* x = dynamic_cast<Derived*>(f.get()))
  {
    for (vector<multiset>::const_iterator j = v.begin(); j != v.end(); ++j)
    {
      x->multiset().push_back(**j);
    }
  }
}

void
update_property(const property& f, const std::vector<property_tag>& v)
{
  for (vector<property_tag>::const_iterator j = v.begin(); j != v.end(); ++j)
  {
    f->property_tag().push_back(**j);
  }
}

void
update_repetition(const multiset& m, const multiset& n, int i)
{
  if (mcc::multiset_repetition* x = dynamic_cast<mcc::multiset_repetition*>(m.get()))
  {
    x->multiset1() = *n.get();
    x->coefficient() = i;
  }
}

template <typename Iterator>
struct mccgrammar
  : qi::grammar<Iterator, property_set(), standard_wide::space_type>
{
  mccgrammar()
  : mccgrammar::base_type(start_, "mcc")
  {
    identifier_ =
          raw[lexeme[((standard_wide::alnum | standard_wide::char_('_')) >> *(standard_wide::alnum | standard_wide::char_('_')))]]
      |   (     qi::lit("\"")
            >>  +(standard_wide::alnum | standard_wide::space | standard_wide::punct - '"')
            >>  qi::lit("\"")
          )
      ;
    value_variable_ =
          identifier_   [ _val = construct<value_variable>(new_<mcc::value_variable>()),
                          bind(&update_name<value_variable, mcc::value_variable>, _val, _1)
                        ]
      ;
    value_ =
          qi::lit("_")  [ _val = construct< ::value >(new_<mcc::value_wildcard>()) ]
      |   identifier_   [ _val = construct< ::value >(new_<mcc::value_constant>()),
                          bind(&update_value< ::value, mcc::value_constant >, _val, _1)
                        ]
      ;
    anonymous_bindings_ =
          qi::eps [ _a = 0, clear(_val) ]
      >> -( (   value_          [ ++_a, _c = _1 ]
            >>  qi::eps         [ _b = construct<value_variable>(new_<mcc::value_variable>()),
                                  bind(&update_auto_name<value_variable, mcc::value_variable>, _b, _a),
                                  push_back(_val, construct<binding>(new_<mcc::binding>(*bind(&value_variable::get, _b), *bind(&::value::get, _c))))
                                ]
            ) % qi::lit(",")
          )
      ;
/*
    named_bindings_ =
          qi::eps [ _a = 0, clear(_val) ]
      >> -( (   value_variable_ [ _b = _1 ]
            >>  qi::lit("=")
            >>  value_          [ _c = _1 ]
            >>  qi::eps         [ push_back(_val, construct<binding>(new_<mcc::binding>(*bind(&value_variable::get, _b), *bind(&::value::get, _c)))) ]
            ) % qi::lit(",")
          )
      ;
*/
    place_ =
          identifier_ [ _val = construct<place>(new_<mcc::place_name>()),
                        bind(&update_name<place, mcc::place_name>, _val, _1)
                      ]
/*
       >> -(     qi::lit("[")
            >>  multiset_expression_ [ _val = construct<place>(new_<mcc::place_restriction>(*bind(&place::get, _val), *bind(&multiset::get, _1))) ]
            >>  qi::lit("]")
          )
*/
      ;
    transition_ =
          identifier_ [ _val = construct<transition>(new_<mcc::transition_name>()),
                        bind(&update_name<transition, mcc::transition_name>, _val, _1)
                      ]
/*
      >> -(     qi::lit("[")
            >>  named_bindings_
                [ _val = construct<transition>(new_<mcc::transition_restriction>(*bind(&transition::get, _val))),
                  bind(&update_bindings<transition, mcc::transition_restriction>, _val, _1)
                ]
            >>  qi::lit("]")
          )
*/
      ;
    simple_multiset_expression_ =
          (     qi::lit("<")
            >>  anonymous_bindings_ [ _a   = construct<term>(new_<mcc::tuple>()),
                                      bind(&update_term, _a, _1),
                                      _val = construct<multiset>(new_<mcc::multiset_constant>(*bind(&term::get, _a)))
                                    ]
            >>  qi::lit(">")
          )
      |   (     qi::lit("marking")
            >>  qi::lit("(")
            >>  place_            [ _val = construct<multiset>(new_<mcc::place_marking>(*bind(&place::get, _1))) ]
            >>  qi::lit(")")
          )
/*
      |   (     qi::lit("set")
            >>  qi::lit("(")
            >>  simple_multiset_expression_ [ _val = construct<multiset>(new_<mcc::set_of>(*bind(&multiset::get, _1))) ]
            >>  qi::lit(")")
          )
*/
/*
      |   (     qi::eps   [ _a = construct<term>(new_<mcc::tuple>()) ]
            >>  int_      [ _b = _1 ]
            >> -("*" >> multiset_expression_ [ _a = _1])
            >>  qi::eps   [ _val = construct<multiset>(new_<mcc::multiset_repetition>()),
                            bind(&update_repetition, _val, _a, _b)
                          ]
          )
*/
      |   qi::lit("(") >> multiset_expression_ [_val = _1] >> qi::lit(")")
      ;
    complex_multiset_expression_ =
          (     qi::eps [clear(_a)]
            >>  (simple_multiset_expression_ [push_back(_a, _1)] >> +(qi::lit("+") >> simple_multiset_expression_ [push_back(_a, _1)]))
            >>  qi::eps [ _val = construct<multiset>(new_<mcc::union_>()),
                          bind(&update_complex_multiset<mcc::union_>, _val, _a)
                        ]
          )
      |   (     qi::eps [clear(_a)]
            >>  (simple_multiset_expression_ [push_back(_a, _1)] >> +(qi::lit("*") >> simple_multiset_expression_ [push_back(_a, _1)]))
            >>  qi::eps [ _val = construct<multiset>(new_<mcc::intersection>()),
                          bind(&update_complex_multiset<mcc::intersection>, _val, _a)
                        ]
          )
      |   (     qi::eps [clear(_a)]
            >>  (simple_multiset_expression_ [push_back(_a, _1)] >> qi::lit("-") >> simple_multiset_expression_ [push_back(_a, _1)])
            >>  qi::eps [ _val = construct<multiset>(new_<mcc::difference>()),
                          bind(&update_complex_multiset<mcc::difference>, _val, _a)
                        ]
          )
      ;
    multiset_expression_ %=
          complex_multiset_expression_
      |   simple_multiset_expression_
      ;
    multiset_comparison_ =
          (     qi::eps [clear(_a)]
            >>  (multiset_expression_ [push_back(_a, _1)] >> qi::lit("=") >> multiset_expression_ [push_back(_a, _1)])
            >>  qi::eps [ _val = construct<formula>(new_<mcc::multiset_eq>()),
                          bind(&update_multiset_comparison<mcc::multiset_eq>, _val, _a)
                        ]
          )
      |   (     qi::eps [clear(_a)]
            >>  (multiset_expression_ [push_back(_a, _1)] >> qi::lit("!=") >> multiset_expression_ [push_back(_a, _1)])
            >>  qi::eps [ _val = construct<formula>(new_<mcc::multiset_ne>()),
                          bind(&update_multiset_comparison<mcc::multiset_ne>, _val, _a)
                        ]

          )
      |   (     qi::eps [clear(_a)]
            >>  (multiset_expression_ [push_back(_a, _1)] >> qi::lit("<") >> multiset_expression_ [push_back(_a, _1)])
            >>  qi::eps [ _val = construct<formula>(new_<mcc::multiset_lt>()),
                          bind(&update_multiset_comparison<mcc::multiset_lt>, _val, _a)
                        ]

          )
      |   (     qi::eps [clear(_a)]
            >>  (multiset_expression_ [push_back(_a, _1)] >> qi::lit("<=") >> multiset_expression_ [push_back(_a, _1)])
            >>  qi::eps [ _val = construct<formula>(new_<mcc::multiset_le>()),
                          bind(&update_multiset_comparison<mcc::multiset_le>, _val, _a)
                        ]

          )
      |   (     qi::eps [clear(_a)]
            >>  (multiset_expression_ [push_back(_a, _1)] >> qi::lit(">") >> multiset_expression_ [push_back(_a, _1)])
            >>  qi::eps [ _val = construct<formula>(new_<mcc::multiset_gt>()),
                          bind(&update_multiset_comparison<mcc::multiset_gt>, _val, _a)
                        ]

          )
      |   (     qi::eps [clear(_a)]
            >>  (multiset_expression_ [push_back(_a, _1)] >> qi::lit(">=") >> multiset_expression_ [push_back(_a, _1)])
            >>  qi::eps [ _val = construct<formula>(new_<mcc::multiset_ge>()),
                          bind(&update_multiset_comparison<mcc::multiset_ge>, _val, _a)
                        ]

          )
      ;
    simple_integer_expression_ =
          int_  [_val = construct<integer>(new_<mcc::integer_constant>()), bind(&update_integer_constant, _val, _1)]
      |   (     qi::lit("bound")
            >>  qi::lit("(")
            >>  place_      [ _val = construct<integer>(new_<mcc::place_bound>(*bind(&place::get, _1))) ]
            >>  qi::lit(")")
          )
      |   (     qi::lit("|")
            >>  multiset_expression_ [ _val = construct<integer>(new_<mcc::multiset_cardinality>(*bind(&multiset::get, _1))) ]
            >>  qi::lit("|")
          )
      |   qi::lit("(") >> integer_expression_ [_val = _1] >> qi::lit(")")
      ;
    complex_integer_expression_ =
          (     qi::eps [clear(_a)]
            >>  (simple_integer_expression_ [push_back(_a, _1)] >> +(qi::lit("+") >> simple_integer_expression_ [push_back(_a, _1)]))
            >>  qi::eps [ _val = construct<integer>(new_<mcc::sum>()),
                          bind(&update_complex_integer<mcc::sum>, _val, _a)
                        ]
          )
      |   (     qi::eps [clear(_a)]
            >>  (simple_integer_expression_ [push_back(_a, _1)] >> +(qi::lit("*") >> simple_integer_expression_ [push_back(_a, _1)]))
            >>  qi::eps [ _val = construct<integer>(new_<mcc::product>()),
                          bind(&update_complex_integer<mcc::product>, _val, _a)
                        ]
          )
      |   (     qi::eps [clear(_a)]
            >>  (simple_integer_expression_ [push_back(_a, _1)] >> qi::lit("-") >> simple_integer_expression_ [push_back(_a, _1)])
            >>  qi::eps [ _val = construct<integer>(new_<mcc::substraction>()),
                          bind(&update_complex_integer<mcc::substraction>, _val, _a)
                        ]
          )
      |   (     qi::eps [clear(_a)]
            >>  (simple_integer_expression_ [push_back(_a, _1)] >> qi::lit("/") >> simple_integer_expression_ [push_back(_a, _1)])
            >>  qi::eps [ _val = construct<integer>(new_<mcc::division>()),
                          bind(&update_complex_integer<mcc::division>, _val, _a)
                        ]
          )
      ;
    integer_expression_ %=
          complex_integer_expression_
      |   simple_integer_expression_
      ;
    integer_comparison_ =
          (     qi::eps [clear(_a)]
            >>  (integer_expression_ [push_back(_a, _1)] >> qi::lit("=") >> integer_expression_ [push_back(_a, _1)])
            >>  qi::eps [ _val = construct<formula>(new_<mcc::integer_eq>()),
                          bind(&update_integer_comparison<mcc::integer_eq>, _val, _a)
                        ]
          )
      |   (     qi::eps [clear(_a)]
            >>  (integer_expression_ [push_back(_a, _1)] >> qi::lit("!=") >> integer_expression_ [push_back(_a, _1)])
            >>  qi::eps [ _val = construct<formula>(new_<mcc::integer_ne>()),
                          bind(&update_integer_comparison<mcc::integer_ne>, _val, _a)
                        ]

          )
      |   (     qi::eps [clear(_a)]
            >>  (integer_expression_ [push_back(_a, _1)] >> qi::lit("<") >> integer_expression_ [push_back(_a, _1)])
            >>  qi::eps [ _val = construct<formula>(new_<mcc::integer_lt>()),
                          bind(&update_integer_comparison<mcc::integer_lt>, _val, _a)
                        ]

          )
      |   (     qi::eps [clear(_a)]
            >>  (integer_expression_ [push_back(_a, _1)] >> qi::lit("<=") >> integer_expression_ [push_back(_a, _1)])
            >>  qi::eps [ _val = construct<formula>(new_<mcc::integer_le>()),
                          bind(&update_integer_comparison<mcc::integer_le>, _val, _a)
                        ]

          )
      |   (     qi::eps [clear(_a)]
            >>  (integer_expression_ [push_back(_a, _1)] >> qi::lit(">") >> integer_expression_ [push_back(_a, _1)])
            >>  qi::eps [ _val = construct<formula>(new_<mcc::integer_gt>()),
                          bind(&update_integer_comparison<mcc::integer_gt>, _val, _a)
                        ]

          )
      |   (     qi::eps [clear(_a)]
            >>  (integer_expression_ [push_back(_a, _1)] >> qi::lit(">=") >> integer_expression_ [push_back(_a, _1)])
            >>  qi::eps [ _val = construct<formula>(new_<mcc::integer_ge>()),
                          bind(&update_integer_comparison<mcc::integer_ge>, _val, _a)
                        ]

          )
      ;
    petri_proposition_ =
          (     qi::lit("deadlock") [ _val = construct<formula>(new_<mcc::is_deadlock>()) ]
          )
      |   (     transition_  [_a = _1]
            >>  qi::lit("?")
            >>  ( qi::lit("l0") [_b = construct<level>(new_<mcc::level>(mcc::level::l0))]
                | qi::lit("l1") [_b = construct<level>(new_<mcc::level>(mcc::level::l1))]
                | qi::lit("l2") [_b = construct<level>(new_<mcc::level>(mcc::level::l2))]
                | qi::lit("l3") [_b = construct<level>(new_<mcc::level>(mcc::level::l3))]
                | qi::lit("l4") [_b = construct<level>(new_<mcc::level>(mcc::level::l4))]
                )
            >>  qi::eps [_val = construct<formula>(new_<mcc::is_live>(*bind(&transition::get, _a), *bind(&level::get, _b)))]
          )
      |   (     transition_  [_a = _1]
            >>  qi::lit("?")
            >>  qi::eps [_val = construct<formula>(new_<mcc::is_fireable>(*bind(&transition::get, _a)))]
          )
      ;
    simple_composite_formula_ =
          qi::lit("true" )  [_val = construct<formula>(new_<mcc::true_ >())]
      |   qi::lit("false")  [_val = construct<formula>(new_<mcc::false_>())]
      |   (     qi::eps [clear(_a)]
            >>  qi::lit("!") >> simple_formula_ [push_back(_a, _1)]
            >>  qi::eps [ _val = construct<formula>(new_<mcc::negation>(*bind(&formula::get, at(_a, 0)))) ]
          )
      ;
    complex_composite_formula_ =
          (     qi::eps [clear(_a)]
            >>  (simple_formula_ [push_back(_a, _1)] >> +(qi::lit("&") >> simple_formula_ [push_back(_a, _1)]))
            >>  qi::eps [ _val = construct<formula>(new_<mcc::conjunction>()),
                          bind(&update_composite<mcc::conjunction>, _val, _a)
                        ]
          )
      |   (     qi::eps [clear(_a)]
            >>  (simple_formula_ [push_back(_a, _1)] >> +(qi::lit("|")  >> simple_formula_ [push_back(_a, _1)]))
            >>  qi::eps [ _val = construct<formula>(new_<mcc::disjunction>()),
                          bind(&update_composite<mcc::disjunction>, _val, _a)
                        ]
          )
      |   (     qi::eps [clear(_a)]
            >>  (simple_formula_ [push_back(_a, _1)] >> +(qi::lit("xor")  >> simple_formula_ [push_back(_a, _1)]))
            >>  qi::eps [ _val = construct<formula>(new_<mcc::exclusive_disjunction>()),
                          bind(&update_composite<mcc::exclusive_disjunction>, _val, _a)
                        ]
          )
      |   (     qi::eps [clear(_a)]
            >>  (simple_formula_ [push_back(_a, _1)] >> qi::lit("=>")  >> simple_formula_ [push_back(_a, _1)])
            >>  qi::eps [ _val = construct<formula>(new_<mcc::implication>()),
                          bind(&update_composite<mcc::implication>, _val, _a)
                        ]
          )
      |   (     qi::eps [clear(_a)]
            >>  (simple_formula_ [push_back(_a, _1)] >> qi::lit("<=>")  >> simple_formula_ [push_back(_a, _1)])
            >>  qi::eps [ _val = construct<formula>(new_<mcc::equivalence>()),
                          bind(&update_composite<mcc::equivalence>, _val, _a)
                        ]
          )
      ;
    system_formula_ =
          (     qi::lit("I")
            >>  simple_formula_ [_val = construct<formula>(new_<mcc::invariant>(*bind(&formula::get, _1)))]
          )
      |   (     qi::lit("N")
            >>  simple_formula_ [_val = construct<formula>(new_<mcc::impossibility>(*bind(&formula::get, _1)))]
          )
      ;
    state_formula_ =
          (     qi::lit("A")
            >>  simple_formula_ [_val = construct<formula>(new_<mcc::all_paths>(*bind(&formula::get, _1)))]
          )
      |   (     qi::lit("E")
            >>  simple_formula_ [_val = construct<formula>(new_<mcc::exists_path>(*bind(&formula::get, _1)))]
          )
      ;
    simple_path_formula_ =
          (     qi::lit("X")  [ _a = true, _b = 1 ]
            >>  -int_         [ _b = _1           ]
            >>  -qi::lit("~") [ _a = false        ]
            >>  simple_formula_ [_val = construct<formula>(new_<mcc::next>(*bind(&formula::get, _1), _a, _b))]
          )
      |   (     qi::lit("G")
            >>  simple_formula_ [_val = construct<formula>(new_<mcc::globally>(*bind(&formula::get, _1)))]
          )
      |   (     qi::lit("F")
            >>  simple_formula_ [_val = construct<formula>(new_<mcc::finally>(*bind(&formula::get, _1)))]
          )
      ;
    complex_path_formula_ =
          (     simple_formula_ [_a = _1]
            >>  qi::lit("U")
            >>  simple_formula_ [_val = construct<formula>(new_<mcc::until>(*bind(&formula::get, _a), *bind(&formula::get, _1), mcc::strength::strong))]
          )
      |   (     simple_formula_ [_a = _1]
            >>  qi::lit("W")
            >>  simple_formula_ [_val = construct<formula>(new_<mcc::until>(*bind(&formula::get, _a), *bind(&formula::get, _1), mcc::strength::weak))]
          )
      ;
    simple_formula_ %=
          simple_composite_formula_
      |   petri_proposition_
      |   integer_comparison_
      |   multiset_comparison_
      |   system_formula_
      |   state_formula_
      |   simple_path_formula_
      |   qi::lit("(") >> formula_ >> qi::lit(")")
      ;
    complex_formula_ %=
          complex_composite_formula_
      |   complex_path_formula_
      ;
    formula_ %=
          complex_formula_
      |   simple_formula_
      ;
    property_ =
          -( qi::lit("ctl")    [ push_back(_c, construct<property_tag>(new_<mcc::ctl_only>())) ]
           | qi::lit("ltl")    [ push_back(_c, construct<property_tag>(new_<mcc::ltl_only>())) ]
           | qi::lit("struct") [ push_back(_c, construct<property_tag>(new_<mcc::structural_only>())) ]
           | qi::lit("reach")  [ push_back(_c, construct<property_tag>(new_<mcc::reachability_only>())) ]
           )
      >>  identifier_   [_a = _1]
      >>  qi::lit(":")
      >>  formula_      [ _b = _1 ]
      >>  qi::eps       [ _val = construct<property>(new_<mcc::property>(_a, *bind(&formula::get, _b))),
                          bind(&update_property, _val, _c)
                        ]
      ;
    properties_ =
          +(property_)  [push_back(_a, _1)]
      >>  qi::eps       [ _val = construct<property_set>(new_<mcc::property_set>()), bind(&update_property_set, _val, _a) ]
      ;
    start_ %=
          properties_
      ;

#   define DEFINE_RULE(v, n) \
    v.name("n"); \
    qi::on_error<qi::fail> \
    ( v \
    , cerr << val("Expecting ") << _4 << val(" here: \"") << construct<string>(_3, _2) << val("\"") << endl \
    );

    DEFINE_RULE(identifier_, "identifier")
    DEFINE_RULE(value_variable_, "value variable")
    DEFINE_RULE(value_, "value")
    DEFINE_RULE(anonymous_bindings_, "anonymous binding")
    DEFINE_RULE(named_bindings_, "named binding")
    DEFINE_RULE(place_, "place")
    DEFINE_RULE(transition_, "transition")
    DEFINE_RULE(simple_multiset_expression_, "simple multiset expression")
    DEFINE_RULE(complex_multiset_expression_, "complex multiset expression")
    DEFINE_RULE(multiset_expression_, "multiset expression")
    DEFINE_RULE(multiset_comparison_, "multiset comparison")
    DEFINE_RULE(simple_integer_expression_, "simple integer expression")
    DEFINE_RULE(complex_integer_expression_, "complex integer expression")
    DEFINE_RULE(integer_expression_, "integer expression")
    DEFINE_RULE(integer_comparison_, "integer comparison")
    DEFINE_RULE(petri_proposition_, "Petri net specific proposition")
    DEFINE_RULE(simple_composite_formula_, "simple composite formula")
    DEFINE_RULE(complex_composite_formula_, "complex composite formula")
    DEFINE_RULE(system_formula_, "system-wide formula")
    DEFINE_RULE(state_formula_, "state formula")
    DEFINE_RULE(simple_path_formula_, "simple path formula")
    DEFINE_RULE(complex_path_formula_, "complex path formula")
    DEFINE_RULE(path_formula_, "path formula")
    DEFINE_RULE(simple_formula_, "simple formula")
    DEFINE_RULE(complex_formula_, "complex formula")
    DEFINE_RULE(formula_, "formula")
    DEFINE_RULE(property_, "property")
    DEFINE_RULE(properties_, "property set")
    DEFINE_RULE(start_, "properties")

#   undef DEFINE_RULE
  }

  qi::rule<Iterator, string()>
    identifier_;
  qi::rule<Iterator, value_variable()>
    value_variable_; 
  qi::rule<Iterator, ::value()>
    value_; 
  qi::rule<Iterator, vector<binding>(), qi::locals<int, value_variable, ::value>, standard_wide::space_type>
    anonymous_bindings_; 
  qi::rule<Iterator, vector<binding>(), qi::locals<int, value_variable, ::value>, standard_wide::space_type>
    named_bindings_; 
  qi::rule<Iterator, place(), qi::locals< vector<binding> >, standard_wide::space_type>
    place_;
  qi::rule<Iterator, transition(), standard_wide::space_type>
    transition_;
  qi::rule<Iterator, multiset(), qi::locals<term, int>, standard_wide::space_type>
    simple_multiset_expression_;
  qi::rule<Iterator, multiset(), qi::locals< vector<multiset> >, standard_wide::space_type>
    complex_multiset_expression_;
  qi::rule<Iterator, multiset(), standard_wide::space_type>
    multiset_expression_;
  qi::rule<Iterator, formula(), qi::locals< vector<multiset> >, standard_wide::space_type>
    multiset_comparison_;
  qi::rule<Iterator, integer(), standard_wide::space_type>
    simple_integer_expression_;
  qi::rule<Iterator, integer(), qi::locals< vector<integer> >, standard_wide::space_type>
    complex_integer_expression_;
  qi::rule<Iterator, integer(), standard_wide::space_type>
    integer_expression_;
  qi::rule<Iterator, formula(), qi::locals< vector<integer> >, standard_wide::space_type>
    integer_comparison_;
  qi::rule<Iterator, formula(), qi::locals< transition, level >, standard_wide::space_type>
    petri_proposition_;
  qi::rule<Iterator, formula(), qi::locals< vector<formula> >, standard_wide::space_type>
    simple_composite_formula_;
  qi::rule<Iterator, formula(), qi::locals< vector<formula> >, standard_wide::space_type>
    complex_composite_formula_;
  qi::rule<Iterator, formula(), standard_wide::space_type>
    system_formula_;
  qi::rule<Iterator, formula(), standard_wide::space_type>
    state_formula_;
  qi::rule<Iterator, formula(), qi::locals<bool, int>, standard_wide::space_type>
    simple_path_formula_;
  qi::rule<Iterator, formula(), qi::locals<formula>, standard_wide::space_type>
    complex_path_formula_;
  qi::rule<Iterator, formula(), standard_wide::space_type>
    path_formula_;
  qi::rule<Iterator, formula(), standard_wide::space_type>
    simple_formula_;
  qi::rule<Iterator, formula(), standard_wide::space_type>
    complex_formula_;
  qi::rule<Iterator, formula(), standard_wide::space_type>
    formula_;
  qi::rule<Iterator, property(), qi::locals< string, formula, vector<property_tag> >, standard_wide::space_type>
    property_;
  qi::rule<Iterator, property_set(), qi::locals< vector<property> >, standard_wide::space_type>
    properties_;
  qi::rule<Iterator, property_set(), standard_wide::space_type>
    start_;
};
