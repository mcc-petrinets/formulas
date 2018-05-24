// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Take the name prefix into account.
#define yylex   tlyylex

// First part of user declarations.

#line 39 "parsetl.cc" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "parsetl.hh"

// User implementation prologue.

#line 53 "parsetl.cc" // lalr1.cc:412
// Unqualified %code blocks.
#line 56 "parsetl.yy" // lalr1.cc:413

/* parsetl.hh and parsedecl.hh include each other recursively.
   We mut ensure that YYSTYPE is declared (by the above %union)
   before parsedecl.hh uses it. */
#include <spot/parsetl/parsedecl.hh>
using namespace spot;

#define missing_right_op_msg(op, str)		\
  error_list.emplace_back(op,			\
    "missing right operand for \"" str "\"");

#define missing_right_op(res, op, str)		\
  do						\
    {						\
      missing_right_op_msg(op, str);		\
      res = fnode::ff();		\
    }						\
  while (0);

// right is missing, so complain and use left.
#define missing_right_binop(res, left, op, str)	\
  do						\
    {						\
      missing_right_op_msg(op, str);		\
      res = left;				\
    }						\
  while (0);

// right is missing, so complain and use false.
#define missing_right_binop_hard(res, left, op, str)	\
  do							\
    {							\
      left->destroy();					\
      missing_right_op(res, op, str);			\
    }							\
  while (0);

  static bool
  sere_ensure_bool(const fnode* f, const spot::location& loc,
                   const char* oper, spot::parse_error_list& error_list)
  {
    if (f->is_boolean())
      return true;
    std::string s;
    s.reserve(80);
    s = "not a Boolean expression: in a SERE ";
    s += oper;
    s += " can only be applied to a Boolean expression";
    error_list.emplace_back(loc, s);
    return false;
  }

  static const fnode*
  error_false_block(const spot::location& loc,
                    spot::parse_error_list& error_list)
  {
    error_list.emplace_back(loc, "treating this block as false");
    return fnode::ff();
  }

  static const fnode*
  parse_ap(const std::string& str,
           const spot::location& location,
           spot::environment& env,
           spot::parse_error_list& error_list)
  {
    auto res = env.require(str);
    if (!res)
      {
        std::string s;
        s.reserve(64);
        s = "unknown atomic proposition `";
        s += str;
        s += "' in ";
        s += env.name();
        error_list.emplace_back(location, s);
      }
    return res.to_node_();
  }

  enum parser_type { parser_ltl, parser_bool, parser_sere };

  static const fnode*
  try_recursive_parse(const std::string& str,
		      const spot::location& location,
		      spot::environment& env,
		      bool debug,
		      parser_type type,
		      spot::parse_error_list& error_list)
    {
      // We want to parse a U (b U c) as two until operators applied
      // to the atomic propositions a, b, and c.  We also want to
      // parse a U (b == c) as one until operator applied to the
      // atomic propositions "a" and "b == c".  The only problem is
      // that we do not know anything about "==" or in general about
      // the syntax of atomic proposition of our users.
      //
      // To support that, the lexer will return "b U c" and "b == c"
      // as PAR_BLOCK tokens.  We then try to parse such tokens
      // recursively.  If, as in the case of "b U c", the block is
      // successfully parsed as a formula, we return this formula.
      // Otherwise, we convert the string into an atomic proposition
      // (it's up to the environment to check the syntax of this
      // proposition, and maybe reject it).

      if (str.empty())
	{
	  error_list.emplace_back(location, "unexpected empty block");
	  return nullptr;
	}

      spot::parsed_formula pf;
      switch (type)
	{
	case parser_sere:
	  pf = spot::parse_infix_sere(str, env, debug, true);
	  break;
	case parser_bool:
	  pf = spot::parse_infix_boolean(str, env, debug, true);
	  break;
	case parser_ltl:
	  pf = spot::parse_infix_psl(str, env, debug, true);
	  break;
	}

      if (pf.errors.empty())
	return pf.f.to_node_();
      return parse_ap(str, location, env, error_list);
    }


#line 187 "parsetl.cc" // lalr1.cc:413


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace tlyy {
#line 273 "parsetl.cc" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  parser::parser (spot::parse_error_list &error_list_yyarg, spot::environment &parse_environment_yyarg, spot::formula &result_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      error_list (error_list_yyarg),
      parse_environment (parse_environment_yyarg),
      result (result_yyarg)
  {}

  parser::~parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
    value = other.value;
  }


  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  inline
  parser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  parser::basic_symbol<Base>::clear ()
  {
    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  parser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
    location = s.location;
  }

  // by_type.
  inline
  parser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  parser::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  inline
  parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  parser::symbol_number_type
  parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
    value = that.value;
    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    switch (yysym.type_get ())
    {
            case 9: // "(...) block"

#line 269 "parsetl.yy" // lalr1.cc:614
        { delete (yysym.value.str); }
#line 528 "parsetl.cc" // lalr1.cc:614
        break;

      case 10: // "{...} block"

#line 269 "parsetl.yy" // lalr1.cc:614
        { delete (yysym.value.str); }
#line 535 "parsetl.cc" // lalr1.cc:614
        break;

      case 11: // "{...}! block"

#line 269 "parsetl.yy" // lalr1.cc:614
        { delete (yysym.value.str); }
#line 542 "parsetl.cc" // lalr1.cc:614
        break;

      case 46: // "atomic proposition"

#line 269 "parsetl.yy" // lalr1.cc:614
        { delete (yysym.value.str); }
#line 549 "parsetl.cc" // lalr1.cc:614
        break;

      case 83: // atomprop

#line 270 "parsetl.yy" // lalr1.cc:614
        { (yysym.value.ltl)->destroy(); }
#line 556 "parsetl.cc" // lalr1.cc:614
        break;

      case 84: // booleanatom

#line 270 "parsetl.yy" // lalr1.cc:614
        { (yysym.value.ltl)->destroy(); }
#line 563 "parsetl.cc" // lalr1.cc:614
        break;

      case 85: // sere

#line 270 "parsetl.yy" // lalr1.cc:614
        { (yysym.value.ltl)->destroy(); }
#line 570 "parsetl.cc" // lalr1.cc:614
        break;

      case 86: // bracedsere

#line 270 "parsetl.yy" // lalr1.cc:614
        { (yysym.value.ltl)->destroy(); }
#line 577 "parsetl.cc" // lalr1.cc:614
        break;

      case 87: // parenthesedsubformula

#line 270 "parsetl.yy" // lalr1.cc:614
        { (yysym.value.ltl)->destroy(); }
#line 584 "parsetl.cc" // lalr1.cc:614
        break;

      case 88: // boolformula

#line 270 "parsetl.yy" // lalr1.cc:614
        { (yysym.value.ltl)->destroy(); }
#line 591 "parsetl.cc" // lalr1.cc:614
        break;

      case 89: // subformula

#line 270 "parsetl.yy" // lalr1.cc:614
        { (yysym.value.ltl)->destroy(); }
#line 598 "parsetl.cc" // lalr1.cc:614
        break;

      case 90: // lbtformula

#line 270 "parsetl.yy" // lalr1.cc:614
        { (yysym.value.ltl)->destroy(); }
#line 605 "parsetl.cc" // lalr1.cc:614
        break;


      default:
        break;
    }
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    switch (yytype)
    {
            case 9: // "(...) block"

#line 272 "parsetl.yy" // lalr1.cc:636
        { debug_stream() << *(yysym.value.str); }
#line 636 "parsetl.cc" // lalr1.cc:636
        break;

      case 10: // "{...} block"

#line 272 "parsetl.yy" // lalr1.cc:636
        { debug_stream() << *(yysym.value.str); }
#line 643 "parsetl.cc" // lalr1.cc:636
        break;

      case 11: // "{...}! block"

#line 272 "parsetl.yy" // lalr1.cc:636
        { debug_stream() << *(yysym.value.str); }
#line 650 "parsetl.cc" // lalr1.cc:636
        break;

      case 39: // "number for square bracket operator"

#line 275 "parsetl.yy" // lalr1.cc:636
        { debug_stream() << (yysym.value.num); }
#line 657 "parsetl.cc" // lalr1.cc:636
        break;

      case 46: // "atomic proposition"

#line 272 "parsetl.yy" // lalr1.cc:636
        { debug_stream() << *(yysym.value.str); }
#line 664 "parsetl.cc" // lalr1.cc:636
        break;

      case 77: // sqbracketargs

#line 276 "parsetl.yy" // lalr1.cc:636
        { debug_stream() << (yysym.value.minmax).min << ".." << (yysym.value.minmax).max; }
#line 671 "parsetl.cc" // lalr1.cc:636
        break;

      case 78: // gotoargs

#line 276 "parsetl.yy" // lalr1.cc:636
        { debug_stream() << (yysym.value.minmax).min << ".." << (yysym.value.minmax).max; }
#line 678 "parsetl.cc" // lalr1.cc:636
        break;

      case 80: // starargs

#line 276 "parsetl.yy" // lalr1.cc:636
        { debug_stream() << (yysym.value.minmax).min << ".." << (yysym.value.minmax).max; }
#line 685 "parsetl.cc" // lalr1.cc:636
        break;

      case 81: // fstarargs

#line 276 "parsetl.yy" // lalr1.cc:636
        { debug_stream() << (yysym.value.minmax).min << ".." << (yysym.value.minmax).max; }
#line 692 "parsetl.cc" // lalr1.cc:636
        break;

      case 82: // equalargs

#line 276 "parsetl.yy" // lalr1.cc:636
        { debug_stream() << (yysym.value.minmax).min << ".." << (yysym.value.minmax).max; }
#line 699 "parsetl.cc" // lalr1.cc:636
        break;

      case 83: // atomprop

#line 273 "parsetl.yy" // lalr1.cc:636
        { print_psl(debug_stream(), formula((yysym.value.ltl)->clone())); }
#line 706 "parsetl.cc" // lalr1.cc:636
        break;

      case 84: // booleanatom

#line 273 "parsetl.yy" // lalr1.cc:636
        { print_psl(debug_stream(), formula((yysym.value.ltl)->clone())); }
#line 713 "parsetl.cc" // lalr1.cc:636
        break;

      case 85: // sere

#line 274 "parsetl.yy" // lalr1.cc:636
        { print_sere(debug_stream(), formula((yysym.value.ltl)->clone())); }
#line 720 "parsetl.cc" // lalr1.cc:636
        break;

      case 86: // bracedsere

#line 274 "parsetl.yy" // lalr1.cc:636
        { print_sere(debug_stream(), formula((yysym.value.ltl)->clone())); }
#line 727 "parsetl.cc" // lalr1.cc:636
        break;

      case 87: // parenthesedsubformula

#line 273 "parsetl.yy" // lalr1.cc:636
        { print_psl(debug_stream(), formula((yysym.value.ltl)->clone())); }
#line 734 "parsetl.cc" // lalr1.cc:636
        break;

      case 88: // boolformula

#line 273 "parsetl.yy" // lalr1.cc:636
        { print_psl(debug_stream(), formula((yysym.value.ltl)->clone())); }
#line 741 "parsetl.cc" // lalr1.cc:636
        break;

      case 89: // subformula

#line 273 "parsetl.yy" // lalr1.cc:636
        { print_psl(debug_stream(), formula((yysym.value.ltl)->clone())); }
#line 748 "parsetl.cc" // lalr1.cc:636
        break;

      case 90: // lbtformula

#line 273 "parsetl.yy" // lalr1.cc:636
        { print_psl(debug_stream(), formula((yysym.value.ltl)->clone())); }
#line 755 "parsetl.cc" // lalr1.cc:636
        break;


      default:
        break;
    }
    yyo << ')';
  }
#endif

  inline
  void
  parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location, error_list));
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 280 "parsetl.yy" // lalr1.cc:859
    {
		result = formula((yystack_[1].value.ltl));
		YYACCEPT;
	      }
#line 978 "parsetl.cc" // lalr1.cc:859
    break;

  case 3:
#line 285 "parsetl.yy" // lalr1.cc:859
    {
		result = nullptr;
		YYABORT;
	      }
#line 987 "parsetl.cc" // lalr1.cc:859
    break;

  case 4:
#line 290 "parsetl.yy" // lalr1.cc:859
    {
		result = formula((yystack_[1].value.ltl));
		YYACCEPT;
	      }
#line 996 "parsetl.cc" // lalr1.cc:859
    break;

  case 5:
#line 295 "parsetl.yy" // lalr1.cc:859
    { YYABORT; }
#line 1002 "parsetl.cc" // lalr1.cc:859
    break;

  case 6:
#line 297 "parsetl.yy" // lalr1.cc:859
    {
		result = formula((yystack_[1].value.ltl));
		YYACCEPT;
	      }
#line 1011 "parsetl.cc" // lalr1.cc:859
    break;

  case 7:
#line 302 "parsetl.yy" // lalr1.cc:859
    {
		result = nullptr;
		YYABORT;
	      }
#line 1020 "parsetl.cc" // lalr1.cc:859
    break;

  case 8:
#line 307 "parsetl.yy" // lalr1.cc:859
    {
		result = formula((yystack_[1].value.ltl));
		YYACCEPT;
	      }
#line 1029 "parsetl.cc" // lalr1.cc:859
    break;

  case 9:
#line 312 "parsetl.yy" // lalr1.cc:859
    { YYABORT; }
#line 1035 "parsetl.cc" // lalr1.cc:859
    break;

  case 10:
#line 314 "parsetl.yy" // lalr1.cc:859
    {
		result = formula((yystack_[1].value.ltl));
		YYACCEPT;
	      }
#line 1044 "parsetl.cc" // lalr1.cc:859
    break;

  case 11:
#line 319 "parsetl.yy" // lalr1.cc:859
    {
		result = nullptr;
		YYABORT;
	      }
#line 1053 "parsetl.cc" // lalr1.cc:859
    break;

  case 12:
#line 324 "parsetl.yy" // lalr1.cc:859
    {
		result = formula((yystack_[1].value.ltl));
		YYACCEPT;
	      }
#line 1062 "parsetl.cc" // lalr1.cc:859
    break;

  case 13:
#line 329 "parsetl.yy" // lalr1.cc:859
    { YYABORT; }
#line 1068 "parsetl.cc" // lalr1.cc:859
    break;

  case 14:
#line 331 "parsetl.yy" // lalr1.cc:859
    {
		result = formula((yystack_[1].value.ltl));
		YYACCEPT;
	      }
#line 1077 "parsetl.cc" // lalr1.cc:859
    break;

  case 15:
#line 336 "parsetl.yy" // lalr1.cc:859
    {
		result = nullptr;
		YYABORT;
	      }
#line 1086 "parsetl.cc" // lalr1.cc:859
    break;

  case 16:
#line 341 "parsetl.yy" // lalr1.cc:859
    {
		result = formula((yystack_[1].value.ltl));
		YYACCEPT;
	      }
#line 1095 "parsetl.cc" // lalr1.cc:859
    break;

  case 17:
#line 346 "parsetl.yy" // lalr1.cc:859
    { YYABORT; }
#line 1101 "parsetl.cc" // lalr1.cc:859
    break;

  case 18:
#line 349 "parsetl.yy" // lalr1.cc:859
    {
		error_list.emplace_back(yylhs.location, "empty input");
		result = nullptr;
	      }
#line 1110 "parsetl.cc" // lalr1.cc:859
    break;

  case 19:
#line 355 "parsetl.yy" // lalr1.cc:859
    {
		error_list.emplace_back(yystack_[1].location, "ignoring trailing garbage");
	      }
#line 1118 "parsetl.cc" // lalr1.cc:859
    break;

  case 26:
#line 366 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.minmax).min = (yystack_[3].value.num); (yylhs.value.minmax).max = (yystack_[1].value.num); }
#line 1124 "parsetl.cc" // lalr1.cc:859
    break;

  case 27:
#line 368 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.minmax).min = (yystack_[2].value.num); (yylhs.value.minmax).max = fnode::unbounded(); }
#line 1130 "parsetl.cc" // lalr1.cc:859
    break;

  case 28:
#line 370 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.minmax).min = 0U; (yylhs.value.minmax).max = (yystack_[1].value.num); }
#line 1136 "parsetl.cc" // lalr1.cc:859
    break;

  case 29:
#line 372 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.minmax).min = 0U; (yylhs.value.minmax).max = fnode::unbounded(); }
#line 1142 "parsetl.cc" // lalr1.cc:859
    break;

  case 30:
#line 374 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.minmax).min = (yylhs.value.minmax).max = (yystack_[1].value.num); }
#line 1148 "parsetl.cc" // lalr1.cc:859
    break;

  case 31:
#line 378 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.minmax).min = (yystack_[3].value.num); (yylhs.value.minmax).max = (yystack_[1].value.num); }
#line 1154 "parsetl.cc" // lalr1.cc:859
    break;

  case 32:
#line 380 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.minmax).min = (yystack_[2].value.num); (yylhs.value.minmax).max = fnode::unbounded(); }
#line 1160 "parsetl.cc" // lalr1.cc:859
    break;

  case 33:
#line 382 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.minmax).min = 1U; (yylhs.value.minmax).max = (yystack_[1].value.num); }
#line 1166 "parsetl.cc" // lalr1.cc:859
    break;

  case 34:
#line 384 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.minmax).min = 1U; (yylhs.value.minmax).max = fnode::unbounded(); }
#line 1172 "parsetl.cc" // lalr1.cc:859
    break;

  case 35:
#line 386 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.minmax).min = (yylhs.value.minmax).max = 1U; }
#line 1178 "parsetl.cc" // lalr1.cc:859
    break;

  case 36:
#line 388 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.minmax).min = (yylhs.value.minmax).max = (yystack_[1].value.num); }
#line 1184 "parsetl.cc" // lalr1.cc:859
    break;

  case 37:
#line 390 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yylhs.location, "treating this goto block as [->]");
             (yylhs.value.minmax).min = (yylhs.value.minmax).max = 1U; }
#line 1191 "parsetl.cc" // lalr1.cc:859
    break;

  case 38:
#line 393 "parsetl.yy" // lalr1.cc:859
    { error_list.
	       emplace_back(yylhs.location, "missing closing bracket for goto operator");
	     (yylhs.value.minmax).min = (yylhs.value.minmax).max = 0U; }
#line 1199 "parsetl.cc" // lalr1.cc:859
    break;

  case 41:
#line 400 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.minmax).min = 0U; (yylhs.value.minmax).max = fnode::unbounded(); }
#line 1205 "parsetl.cc" // lalr1.cc:859
    break;

  case 42:
#line 402 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.minmax).min = 1U; (yylhs.value.minmax).max = fnode::unbounded(); }
#line 1211 "parsetl.cc" // lalr1.cc:859
    break;

  case 43:
#line 404 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.minmax) = (yystack_[0].value.minmax); }
#line 1217 "parsetl.cc" // lalr1.cc:859
    break;

  case 44:
#line 406 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yylhs.location, "treating this star block as [*]");
              (yylhs.value.minmax).min = 0U; (yylhs.value.minmax).max = fnode::unbounded(); }
#line 1224 "parsetl.cc" // lalr1.cc:859
    break;

  case 45:
#line 409 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yylhs.location, "missing closing bracket for star");
	      (yylhs.value.minmax).min = (yylhs.value.minmax).max = 0U; }
#line 1231 "parsetl.cc" // lalr1.cc:859
    break;

  case 46:
#line 413 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.minmax).min = 0U; (yylhs.value.minmax).max = fnode::unbounded(); }
#line 1237 "parsetl.cc" // lalr1.cc:859
    break;

  case 47:
#line 415 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.minmax).min = 1U; (yylhs.value.minmax).max = fnode::unbounded(); }
#line 1243 "parsetl.cc" // lalr1.cc:859
    break;

  case 48:
#line 417 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.minmax) = (yystack_[0].value.minmax); }
#line 1249 "parsetl.cc" // lalr1.cc:859
    break;

  case 49:
#line 419 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back
		(yylhs.location, "treating this fusion-star block as [:*]");
              (yylhs.value.minmax).min = 0U; (yylhs.value.minmax).max = fnode::unbounded(); }
#line 1257 "parsetl.cc" // lalr1.cc:859
    break;

  case 50:
#line 423 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back
		(yylhs.location, "missing closing bracket for fusion-star");
	      (yylhs.value.minmax).min = (yylhs.value.minmax).max = 0U; }
#line 1265 "parsetl.cc" // lalr1.cc:859
    break;

  case 51:
#line 428 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.minmax) = (yystack_[0].value.minmax); }
#line 1271 "parsetl.cc" // lalr1.cc:859
    break;

  case 52:
#line 430 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yylhs.location, "treating this equal block as [=]");
              (yylhs.value.minmax).min = 0U; (yylhs.value.minmax).max = fnode::unbounded(); }
#line 1278 "parsetl.cc" // lalr1.cc:859
    break;

  case 53:
#line 433 "parsetl.yy" // lalr1.cc:859
    { error_list.
		emplace_back(yylhs.location, "missing closing bracket for equal operator");
	      (yylhs.value.minmax).min = (yylhs.value.minmax).max = 0U; }
#line 1286 "parsetl.cc" // lalr1.cc:859
    break;

  case 54:
#line 439 "parsetl.yy" // lalr1.cc:859
    {
            (yylhs.value.ltl) = parse_ap(*(yystack_[0].value.str), yystack_[0].location, parse_environment, error_list);
            delete (yystack_[0].value.str);
            if (!(yylhs.value.ltl))
              YYERROR;
          }
#line 1297 "parsetl.cc" // lalr1.cc:859
    break;

  case 57:
#line 449 "parsetl.yy" // lalr1.cc:859
    {
		(yylhs.value.ltl) = fnode::unop(op::Not, (yystack_[1].value.ltl));
	      }
#line 1305 "parsetl.cc" // lalr1.cc:859
    break;

  case 58:
#line 453 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::tt(); }
#line 1311 "parsetl.cc" // lalr1.cc:859
    break;

  case 59:
#line 455 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::ff(); }
#line 1317 "parsetl.cc" // lalr1.cc:859
    break;

  case 61:
#line 459 "parsetl.yy" // lalr1.cc:859
    {
		if (sere_ensure_bool((yystack_[0].value.ltl), yystack_[0].location, "`!'", error_list))
		  {
		    (yylhs.value.ltl) = fnode::unop(op::Not, (yystack_[0].value.ltl));
		  }
		else
		  {
		    (yystack_[0].value.ltl)->destroy();
		    (yylhs.value.ltl) = error_false_block(yylhs.location, error_list);
		  }
	      }
#line 1333 "parsetl.cc" // lalr1.cc:859
    break;

  case 63:
#line 472 "parsetl.yy" // lalr1.cc:859
    {
		(yylhs.value.ltl) =
		  try_recursive_parse(*(yystack_[0].value.str), yystack_[0].location, parse_environment,
				      debug_level(), parser_sere, error_list);
		delete (yystack_[0].value.str);
		if (!(yylhs.value.ltl))
		  YYERROR;
	      }
#line 1346 "parsetl.cc" // lalr1.cc:859
    break;

  case 64:
#line 481 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = (yystack_[1].value.ltl); }
#line 1352 "parsetl.cc" // lalr1.cc:859
    break;

  case 65:
#line 483 "parsetl.yy" // lalr1.cc:859
    { error_list.
		  emplace_back(yylhs.location,
			       "treating this parenthetical block as false");
		(yylhs.value.ltl) = fnode::ff();
	      }
#line 1362 "parsetl.cc" // lalr1.cc:859
    break;

  case 66:
#line 489 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yystack_[2].location + yystack_[1].location, "missing closing parenthesis");
		(yylhs.value.ltl) = (yystack_[1].value.ltl);
	      }
#line 1370 "parsetl.cc" // lalr1.cc:859
    break;

  case 67:
#line 493 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yylhs.location,
                    "missing closing parenthesis, "
		    "treating this parenthetical block as false");
		(yylhs.value.ltl) = fnode::ff();
	      }
#line 1380 "parsetl.cc" // lalr1.cc:859
    break;

  case 68:
#line 499 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::multop(op::AndRat, {(yystack_[2].value.ltl), (yystack_[0].value.ltl)}); }
#line 1386 "parsetl.cc" // lalr1.cc:859
    break;

  case 69:
#line 501 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location,
				    "length-matching and operator"); }
#line 1393 "parsetl.cc" // lalr1.cc:859
    break;

  case 70:
#line 504 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::multop(op::AndNLM, {(yystack_[2].value.ltl), (yystack_[0].value.ltl)}); }
#line 1399 "parsetl.cc" // lalr1.cc:859
    break;

  case 71:
#line 506 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location,
                                    "non-length-matching and operator"); }
#line 1406 "parsetl.cc" // lalr1.cc:859
    break;

  case 72:
#line 509 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::multop(op::OrRat, {(yystack_[2].value.ltl), (yystack_[0].value.ltl)}); }
#line 1412 "parsetl.cc" // lalr1.cc:859
    break;

  case 73:
#line 511 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "or operator"); }
#line 1418 "parsetl.cc" // lalr1.cc:859
    break;

  case 74:
#line 513 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::multop(op::Concat, {(yystack_[2].value.ltl), (yystack_[0].value.ltl)}); }
#line 1424 "parsetl.cc" // lalr1.cc:859
    break;

  case 75:
#line 515 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "concat operator"); }
#line 1430 "parsetl.cc" // lalr1.cc:859
    break;

  case 76:
#line 517 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::multop(op::Fusion, {(yystack_[2].value.ltl), (yystack_[0].value.ltl)}); }
#line 1436 "parsetl.cc" // lalr1.cc:859
    break;

  case 77:
#line 519 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "fusion operator"); }
#line 1442 "parsetl.cc" // lalr1.cc:859
    break;

  case 78:
#line 521 "parsetl.yy" // lalr1.cc:859
    {
		if ((yystack_[0].value.minmax).max < (yystack_[0].value.minmax).min)
		  {
		    error_list.emplace_back(yystack_[0].location, "reversed range");
		    std::swap((yystack_[0].value.minmax).max, (yystack_[0].value.minmax).min);
		  }
		(yylhs.value.ltl) = fnode::bunop(op::Star, fnode::tt(), (yystack_[0].value.minmax).min, (yystack_[0].value.minmax).max);
	      }
#line 1455 "parsetl.cc" // lalr1.cc:859
    break;

  case 79:
#line 530 "parsetl.yy" // lalr1.cc:859
    {
		if ((yystack_[0].value.minmax).max < (yystack_[0].value.minmax).min)
		  {
		    error_list.emplace_back(yystack_[0].location, "reversed range");
		    std::swap((yystack_[0].value.minmax).max, (yystack_[0].value.minmax).min);
		  }
		(yylhs.value.ltl) = fnode::bunop(op::Star, (yystack_[1].value.ltl), (yystack_[0].value.minmax).min, (yystack_[0].value.minmax).max);
	      }
#line 1468 "parsetl.cc" // lalr1.cc:859
    break;

  case 80:
#line 539 "parsetl.yy" // lalr1.cc:859
    {
		if ((yystack_[0].value.minmax).max < (yystack_[0].value.minmax).min)
		  {
		    error_list.emplace_back(yystack_[0].location, "reversed range");
		    std::swap((yystack_[0].value.minmax).max, (yystack_[0].value.minmax).min);
		  }
		(yylhs.value.ltl) = fnode::bunop(op::FStar, (yystack_[1].value.ltl), (yystack_[0].value.minmax).min, (yystack_[0].value.minmax).max);
	      }
#line 1481 "parsetl.cc" // lalr1.cc:859
    break;

  case 81:
#line 548 "parsetl.yy" // lalr1.cc:859
    {
		if ((yystack_[0].value.minmax).max < (yystack_[0].value.minmax).min)
		  {
		    error_list.emplace_back(yystack_[0].location, "reversed range");
		    std::swap((yystack_[0].value.minmax).max, (yystack_[0].value.minmax).min);
		  }
		if (sere_ensure_bool((yystack_[1].value.ltl), yystack_[1].location, "[=...]", error_list))
		  {
		    (yylhs.value.ltl) = formula::sugar_equal(formula((yystack_[1].value.ltl)),
					      (yystack_[0].value.minmax).min, (yystack_[0].value.minmax).max).to_node_();
		  }
		else
		  {
		    (yystack_[1].value.ltl)->destroy();
		    (yylhs.value.ltl) = error_false_block(yylhs.location, error_list);
		  }
	      }
#line 1503 "parsetl.cc" // lalr1.cc:859
    break;

  case 82:
#line 566 "parsetl.yy" // lalr1.cc:859
    {
		if ((yystack_[0].value.minmax).max < (yystack_[0].value.minmax).min)
		  {
		    error_list.emplace_back(yystack_[0].location, "reversed range");
		    std::swap((yystack_[0].value.minmax).max, (yystack_[0].value.minmax).min);
		  }
		if (sere_ensure_bool((yystack_[1].value.ltl), yystack_[1].location, "[->...]", error_list))
		  {
		    (yylhs.value.ltl) = formula::sugar_goto(formula((yystack_[1].value.ltl)),
					     (yystack_[0].value.minmax).min, (yystack_[0].value.minmax).max).to_node_();
		  }
		else
		  {
		    (yystack_[1].value.ltl)->destroy();
		    (yylhs.value.ltl) = error_false_block(yylhs.location, error_list);
		  }
	      }
#line 1525 "parsetl.cc" // lalr1.cc:859
    break;

  case 83:
#line 584 "parsetl.yy" // lalr1.cc:859
    {
		if (sere_ensure_bool((yystack_[2].value.ltl), yystack_[2].location, "`^'", error_list)
                    && sere_ensure_bool((yystack_[0].value.ltl), yystack_[0].location, "`^'", error_list))
		  {
		    (yylhs.value.ltl) = fnode::binop(op::Xor, (yystack_[2].value.ltl), (yystack_[0].value.ltl));
		  }
		else
		  {
		    (yystack_[2].value.ltl)->destroy();
		    (yystack_[0].value.ltl)->destroy();
		    (yylhs.value.ltl) = error_false_block(yylhs.location, error_list);
		  }
	      }
#line 1543 "parsetl.cc" // lalr1.cc:859
    break;

  case 84:
#line 598 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "xor operator"); }
#line 1549 "parsetl.cc" // lalr1.cc:859
    break;

  case 85:
#line 600 "parsetl.yy" // lalr1.cc:859
    {
		if (sere_ensure_bool((yystack_[2].value.ltl), yystack_[2].location, "`->'", error_list))
		  {
		    (yylhs.value.ltl) = fnode::binop(op::Implies, (yystack_[2].value.ltl), (yystack_[0].value.ltl));
		  }
		else
		  {
		    (yystack_[2].value.ltl)->destroy();
		    (yystack_[0].value.ltl)->destroy();
		    (yylhs.value.ltl) = error_false_block(yylhs.location, error_list);
		  }
	      }
#line 1566 "parsetl.cc" // lalr1.cc:859
    break;

  case 86:
#line 613 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "implication operator"); }
#line 1572 "parsetl.cc" // lalr1.cc:859
    break;

  case 87:
#line 615 "parsetl.yy" // lalr1.cc:859
    {
		if (sere_ensure_bool((yystack_[2].value.ltl), yystack_[2].location, "`<->'", error_list)
                    && sere_ensure_bool((yystack_[0].value.ltl), yystack_[0].location, "`<->'", error_list))
		  {
		    (yylhs.value.ltl) = fnode::binop(op::Equiv, (yystack_[2].value.ltl), (yystack_[0].value.ltl));
		  }
		else
		  {
		    (yystack_[2].value.ltl)->destroy();
		    (yystack_[0].value.ltl)->destroy();
		    (yylhs.value.ltl) = error_false_block(yylhs.location, error_list);
		  }
	      }
#line 1590 "parsetl.cc" // lalr1.cc:859
    break;

  case 88:
#line 629 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "equivalent operator"); }
#line 1596 "parsetl.cc" // lalr1.cc:859
    break;

  case 89:
#line 632 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = (yystack_[1].value.ltl); }
#line 1602 "parsetl.cc" // lalr1.cc:859
    break;

  case 90:
#line 634 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yystack_[1].location, "ignoring this");
		(yylhs.value.ltl) = (yystack_[2].value.ltl);
	      }
#line 1610 "parsetl.cc" // lalr1.cc:859
    break;

  case 91:
#line 638 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yylhs.location,
					"treating this brace block as false");
		(yylhs.value.ltl) = fnode::ff();
	      }
#line 1619 "parsetl.cc" // lalr1.cc:859
    break;

  case 92:
#line 643 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yystack_[2].location + yystack_[1].location,
					"missing closing brace");
		(yylhs.value.ltl) = (yystack_[1].value.ltl);
	      }
#line 1628 "parsetl.cc" // lalr1.cc:859
    break;

  case 93:
#line 648 "parsetl.yy" // lalr1.cc:859
    { error_list. emplace_back(yystack_[1].location,
                  "ignoring trailing garbage and missing closing brace");
		(yylhs.value.ltl) = (yystack_[2].value.ltl);
	      }
#line 1637 "parsetl.cc" // lalr1.cc:859
    break;

  case 94:
#line 653 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yylhs.location,
                    "missing closing brace, "
		    "treating this brace block as false");
		(yylhs.value.ltl) = fnode::ff();
	      }
#line 1647 "parsetl.cc" // lalr1.cc:859
    break;

  case 95:
#line 659 "parsetl.yy" // lalr1.cc:859
    {
		(yylhs.value.ltl) = try_recursive_parse(*(yystack_[0].value.str), yystack_[0].location, parse_environment,
					 debug_level(),
                                         parser_sere, error_list);
		delete (yystack_[0].value.str);
		if (!(yylhs.value.ltl))
		  YYERROR;
	      }
#line 1660 "parsetl.cc" // lalr1.cc:859
    break;

  case 96:
#line 669 "parsetl.yy" // lalr1.cc:859
    {
		(yylhs.value.ltl) = try_recursive_parse(*(yystack_[0].value.str), yystack_[0].location, parse_environment,
					 debug_level(), parser_ltl, error_list);
		delete (yystack_[0].value.str);
		if (!(yylhs.value.ltl))
		  YYERROR;
	      }
#line 1672 "parsetl.cc" // lalr1.cc:859
    break;

  case 97:
#line 677 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = (yystack_[1].value.ltl); }
#line 1678 "parsetl.cc" // lalr1.cc:859
    break;

  case 98:
#line 679 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yystack_[1].location, "ignoring this");
		(yylhs.value.ltl) = (yystack_[2].value.ltl);
	      }
#line 1686 "parsetl.cc" // lalr1.cc:859
    break;

  case 99:
#line 683 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yylhs.location,
		 "treating this parenthetical block as false");
		(yylhs.value.ltl) = fnode::ff();
	      }
#line 1695 "parsetl.cc" // lalr1.cc:859
    break;

  case 100:
#line 688 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yystack_[2].location + yystack_[1].location, "missing closing parenthesis");
		(yylhs.value.ltl) = (yystack_[1].value.ltl);
	      }
#line 1703 "parsetl.cc" // lalr1.cc:859
    break;

  case 101:
#line 692 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yystack_[1].location,
                "ignoring trailing garbage and missing closing parenthesis");
		(yylhs.value.ltl) = (yystack_[2].value.ltl);
	      }
#line 1712 "parsetl.cc" // lalr1.cc:859
    break;

  case 102:
#line 697 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yylhs.location,
                    "missing closing parenthesis, "
		    "treating this parenthetical block as false");
		(yylhs.value.ltl) = fnode::ff();
	      }
#line 1722 "parsetl.cc" // lalr1.cc:859
    break;

  case 104:
#line 706 "parsetl.yy" // lalr1.cc:859
    {
		(yylhs.value.ltl) = try_recursive_parse(*(yystack_[0].value.str), yystack_[0].location, parse_environment,
					 debug_level(),
                                         parser_bool, error_list);
		delete (yystack_[0].value.str);
		if (!(yylhs.value.ltl))
		  YYERROR;
	      }
#line 1735 "parsetl.cc" // lalr1.cc:859
    break;

  case 105:
#line 715 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = (yystack_[1].value.ltl); }
#line 1741 "parsetl.cc" // lalr1.cc:859
    break;

  case 106:
#line 717 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yystack_[1].location, "ignoring this");
		(yylhs.value.ltl) = (yystack_[2].value.ltl);
	      }
#line 1749 "parsetl.cc" // lalr1.cc:859
    break;

  case 107:
#line 721 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yylhs.location,
		 "treating this parenthetical block as false");
		(yylhs.value.ltl) = fnode::ff();
	      }
#line 1758 "parsetl.cc" // lalr1.cc:859
    break;

  case 108:
#line 726 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yystack_[2].location + yystack_[1].location,
					"missing closing parenthesis");
		(yylhs.value.ltl) = (yystack_[1].value.ltl);
	      }
#line 1767 "parsetl.cc" // lalr1.cc:859
    break;

  case 109:
#line 731 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yystack_[1].location,
                "ignoring trailing garbage and missing closing parenthesis");
		(yylhs.value.ltl) = (yystack_[2].value.ltl);
	      }
#line 1776 "parsetl.cc" // lalr1.cc:859
    break;

  case 110:
#line 736 "parsetl.yy" // lalr1.cc:859
    { error_list.emplace_back(yylhs.location,
                    "missing closing parenthesis, "
		    "treating this parenthetical block as false");
		(yylhs.value.ltl) = fnode::ff();
	      }
#line 1786 "parsetl.cc" // lalr1.cc:859
    break;

  case 111:
#line 742 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::multop(op::And, {(yystack_[2].value.ltl), (yystack_[0].value.ltl)}); }
#line 1792 "parsetl.cc" // lalr1.cc:859
    break;

  case 112:
#line 744 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "and operator"); }
#line 1798 "parsetl.cc" // lalr1.cc:859
    break;

  case 113:
#line 746 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::multop(op::And, {(yystack_[2].value.ltl), (yystack_[0].value.ltl)}); }
#line 1804 "parsetl.cc" // lalr1.cc:859
    break;

  case 114:
#line 748 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "and operator"); }
#line 1810 "parsetl.cc" // lalr1.cc:859
    break;

  case 115:
#line 750 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::multop(op::And, {(yystack_[2].value.ltl), (yystack_[0].value.ltl)}); }
#line 1816 "parsetl.cc" // lalr1.cc:859
    break;

  case 116:
#line 752 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "and operator"); }
#line 1822 "parsetl.cc" // lalr1.cc:859
    break;

  case 117:
#line 754 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::multop(op::Or, {(yystack_[2].value.ltl), (yystack_[0].value.ltl)}); }
#line 1828 "parsetl.cc" // lalr1.cc:859
    break;

  case 118:
#line 756 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "or operator"); }
#line 1834 "parsetl.cc" // lalr1.cc:859
    break;

  case 119:
#line 758 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::Xor, (yystack_[2].value.ltl), (yystack_[0].value.ltl)); }
#line 1840 "parsetl.cc" // lalr1.cc:859
    break;

  case 120:
#line 760 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "xor operator"); }
#line 1846 "parsetl.cc" // lalr1.cc:859
    break;

  case 121:
#line 762 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::Implies, (yystack_[2].value.ltl), (yystack_[0].value.ltl)); }
#line 1852 "parsetl.cc" // lalr1.cc:859
    break;

  case 122:
#line 764 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "implication operator"); }
#line 1858 "parsetl.cc" // lalr1.cc:859
    break;

  case 123:
#line 766 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::Equiv, (yystack_[2].value.ltl), (yystack_[0].value.ltl)); }
#line 1864 "parsetl.cc" // lalr1.cc:859
    break;

  case 124:
#line 768 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "equivalent operator"); }
#line 1870 "parsetl.cc" // lalr1.cc:859
    break;

  case 125:
#line 770 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::unop(op::Not, (yystack_[0].value.ltl)); }
#line 1876 "parsetl.cc" // lalr1.cc:859
    break;

  case 126:
#line 772 "parsetl.yy" // lalr1.cc:859
    { missing_right_op((yylhs.value.ltl), yystack_[1].location, "not operator"); }
#line 1882 "parsetl.cc" // lalr1.cc:859
    break;

  case 129:
#line 777 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::multop(op::And, {(yystack_[2].value.ltl), (yystack_[0].value.ltl)}); }
#line 1888 "parsetl.cc" // lalr1.cc:859
    break;

  case 130:
#line 779 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "and operator"); }
#line 1894 "parsetl.cc" // lalr1.cc:859
    break;

  case 131:
#line 781 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::multop(op::And, {(yystack_[2].value.ltl), (yystack_[0].value.ltl)}); }
#line 1900 "parsetl.cc" // lalr1.cc:859
    break;

  case 132:
#line 783 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "and operator"); }
#line 1906 "parsetl.cc" // lalr1.cc:859
    break;

  case 133:
#line 785 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::multop(op::And, {(yystack_[2].value.ltl), (yystack_[0].value.ltl)}); }
#line 1912 "parsetl.cc" // lalr1.cc:859
    break;

  case 134:
#line 787 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "and operator"); }
#line 1918 "parsetl.cc" // lalr1.cc:859
    break;

  case 135:
#line 789 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::multop(op::Or, {(yystack_[2].value.ltl), (yystack_[0].value.ltl)}); }
#line 1924 "parsetl.cc" // lalr1.cc:859
    break;

  case 136:
#line 791 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "or operator"); }
#line 1930 "parsetl.cc" // lalr1.cc:859
    break;

  case 137:
#line 793 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::Xor, (yystack_[2].value.ltl), (yystack_[0].value.ltl)); }
#line 1936 "parsetl.cc" // lalr1.cc:859
    break;

  case 138:
#line 795 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "xor operator"); }
#line 1942 "parsetl.cc" // lalr1.cc:859
    break;

  case 139:
#line 797 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::Implies, (yystack_[2].value.ltl), (yystack_[0].value.ltl)); }
#line 1948 "parsetl.cc" // lalr1.cc:859
    break;

  case 140:
#line 799 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "implication operator"); }
#line 1954 "parsetl.cc" // lalr1.cc:859
    break;

  case 141:
#line 801 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::Equiv, (yystack_[2].value.ltl), (yystack_[0].value.ltl)); }
#line 1960 "parsetl.cc" // lalr1.cc:859
    break;

  case 142:
#line 803 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "equivalent operator"); }
#line 1966 "parsetl.cc" // lalr1.cc:859
    break;

  case 143:
#line 805 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::U, (yystack_[2].value.ltl), (yystack_[0].value.ltl)); }
#line 1972 "parsetl.cc" // lalr1.cc:859
    break;

  case 144:
#line 807 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "until operator"); }
#line 1978 "parsetl.cc" // lalr1.cc:859
    break;

  case 145:
#line 809 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::R, (yystack_[2].value.ltl), (yystack_[0].value.ltl)); }
#line 1984 "parsetl.cc" // lalr1.cc:859
    break;

  case 146:
#line 811 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "release operator"); }
#line 1990 "parsetl.cc" // lalr1.cc:859
    break;

  case 147:
#line 813 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::W, (yystack_[2].value.ltl), (yystack_[0].value.ltl)); }
#line 1996 "parsetl.cc" // lalr1.cc:859
    break;

  case 148:
#line 815 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "weak until operator"); }
#line 2002 "parsetl.cc" // lalr1.cc:859
    break;

  case 149:
#line 817 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::M, (yystack_[2].value.ltl), (yystack_[0].value.ltl)); }
#line 2008 "parsetl.cc" // lalr1.cc:859
    break;

  case 150:
#line 819 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location, "strong release operator"); }
#line 2014 "parsetl.cc" // lalr1.cc:859
    break;

  case 151:
#line 821 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::unop(op::F, (yystack_[0].value.ltl)); }
#line 2020 "parsetl.cc" // lalr1.cc:859
    break;

  case 152:
#line 823 "parsetl.yy" // lalr1.cc:859
    { missing_right_op((yylhs.value.ltl), yystack_[1].location, "sometimes operator"); }
#line 2026 "parsetl.cc" // lalr1.cc:859
    break;

  case 153:
#line 825 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::unop(op::G, (yystack_[0].value.ltl)); }
#line 2032 "parsetl.cc" // lalr1.cc:859
    break;

  case 154:
#line 827 "parsetl.yy" // lalr1.cc:859
    { missing_right_op((yylhs.value.ltl), yystack_[1].location, "always operator"); }
#line 2038 "parsetl.cc" // lalr1.cc:859
    break;

  case 155:
#line 829 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::unop(op::X, (yystack_[0].value.ltl)); }
#line 2044 "parsetl.cc" // lalr1.cc:859
    break;

  case 156:
#line 831 "parsetl.yy" // lalr1.cc:859
    { missing_right_op((yylhs.value.ltl), yystack_[1].location, "next operator"); }
#line 2050 "parsetl.cc" // lalr1.cc:859
    break;

  case 157:
#line 833 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::unop(op::Not, (yystack_[0].value.ltl)); }
#line 2056 "parsetl.cc" // lalr1.cc:859
    break;

  case 158:
#line 835 "parsetl.yy" // lalr1.cc:859
    { missing_right_op((yylhs.value.ltl), yystack_[1].location, "not operator"); }
#line 2062 "parsetl.cc" // lalr1.cc:859
    break;

  case 159:
#line 837 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::unop(op::Closure, (yystack_[0].value.ltl)); }
#line 2068 "parsetl.cc" // lalr1.cc:859
    break;

  case 160:
#line 839 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::UConcat, (yystack_[2].value.ltl), (yystack_[0].value.ltl)); }
#line 2074 "parsetl.cc" // lalr1.cc:859
    break;

  case 161:
#line 841 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::UConcat, (yystack_[1].value.ltl), (yystack_[0].value.ltl)); }
#line 2080 "parsetl.cc" // lalr1.cc:859
    break;

  case 162:
#line 843 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop_hard((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location,
				    "universal overlapping concat operator"); }
#line 2087 "parsetl.cc" // lalr1.cc:859
    break;

  case 163:
#line 846 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::EConcat, (yystack_[2].value.ltl), (yystack_[0].value.ltl)); }
#line 2093 "parsetl.cc" // lalr1.cc:859
    break;

  case 164:
#line 848 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop_hard((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location,
				    "existential overlapping concat operator");
	      }
#line 2101 "parsetl.cc" // lalr1.cc:859
    break;

  case 165:
#line 853 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::UConcat,
				  fnode::multop(op::Concat, {(yystack_[2].value.ltl), fnode::tt()}),
				  (yystack_[0].value.ltl)); }
#line 2109 "parsetl.cc" // lalr1.cc:859
    break;

  case 166:
#line 857 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop_hard((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location,
				  "universal non-overlapping concat operator");
	      }
#line 2117 "parsetl.cc" // lalr1.cc:859
    break;

  case 167:
#line 862 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::EConcat,
				  fnode::multop(op::Concat, {(yystack_[2].value.ltl), fnode::tt()}),
				  (yystack_[0].value.ltl)); }
#line 2125 "parsetl.cc" // lalr1.cc:859
    break;

  case 168:
#line 866 "parsetl.yy" // lalr1.cc:859
    { missing_right_binop_hard((yylhs.value.ltl), (yystack_[2].value.ltl), yystack_[1].location,
				"existential non-overlapping concat operator");
	      }
#line 2133 "parsetl.cc" // lalr1.cc:859
    break;

  case 169:
#line 871 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::EConcat, (yystack_[1].value.ltl), fnode::tt()); }
#line 2139 "parsetl.cc" // lalr1.cc:859
    break;

  case 170:
#line 873 "parsetl.yy" // lalr1.cc:859
    {
		(yylhs.value.ltl) = try_recursive_parse(*(yystack_[0].value.str), yystack_[0].location, parse_environment,
					 debug_level(),
                                         parser_sere, error_list);
		delete (yystack_[0].value.str);
		if (!(yylhs.value.ltl))
		  YYERROR;
		(yylhs.value.ltl) = fnode::binop(op::EConcat, (yylhs.value.ltl), fnode::tt());
	      }
#line 2153 "parsetl.cc" // lalr1.cc:859
    break;

  case 172:
#line 885 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::unop(op::Not, (yystack_[0].value.ltl)); }
#line 2159 "parsetl.cc" // lalr1.cc:859
    break;

  case 173:
#line 887 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::multop(op::And, {(yystack_[1].value.ltl), (yystack_[0].value.ltl)}); }
#line 2165 "parsetl.cc" // lalr1.cc:859
    break;

  case 174:
#line 889 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::multop(op::Or, {(yystack_[1].value.ltl), (yystack_[0].value.ltl)}); }
#line 2171 "parsetl.cc" // lalr1.cc:859
    break;

  case 175:
#line 891 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::Xor, (yystack_[1].value.ltl), (yystack_[0].value.ltl)); }
#line 2177 "parsetl.cc" // lalr1.cc:859
    break;

  case 176:
#line 893 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::Implies, (yystack_[1].value.ltl), (yystack_[0].value.ltl)); }
#line 2183 "parsetl.cc" // lalr1.cc:859
    break;

  case 177:
#line 895 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::Equiv, (yystack_[1].value.ltl), (yystack_[0].value.ltl)); }
#line 2189 "parsetl.cc" // lalr1.cc:859
    break;

  case 178:
#line 897 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::unop(op::X, (yystack_[0].value.ltl)); }
#line 2195 "parsetl.cc" // lalr1.cc:859
    break;

  case 179:
#line 899 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::unop(op::F, (yystack_[0].value.ltl)); }
#line 2201 "parsetl.cc" // lalr1.cc:859
    break;

  case 180:
#line 901 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::unop(op::G, (yystack_[0].value.ltl)); }
#line 2207 "parsetl.cc" // lalr1.cc:859
    break;

  case 181:
#line 903 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::U, (yystack_[1].value.ltl), (yystack_[0].value.ltl)); }
#line 2213 "parsetl.cc" // lalr1.cc:859
    break;

  case 182:
#line 905 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::R, (yystack_[1].value.ltl), (yystack_[0].value.ltl)); }
#line 2219 "parsetl.cc" // lalr1.cc:859
    break;

  case 183:
#line 907 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::R, (yystack_[1].value.ltl), (yystack_[0].value.ltl)); }
#line 2225 "parsetl.cc" // lalr1.cc:859
    break;

  case 184:
#line 909 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::W, (yystack_[1].value.ltl), (yystack_[0].value.ltl)); }
#line 2231 "parsetl.cc" // lalr1.cc:859
    break;

  case 185:
#line 911 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::binop(op::M, (yystack_[1].value.ltl), (yystack_[0].value.ltl)); }
#line 2237 "parsetl.cc" // lalr1.cc:859
    break;

  case 186:
#line 913 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::tt(); }
#line 2243 "parsetl.cc" // lalr1.cc:859
    break;

  case 187:
#line 915 "parsetl.yy" // lalr1.cc:859
    { (yylhs.value.ltl) = fnode::ff(); }
#line 2249 "parsetl.cc" // lalr1.cc:859
    break;


#line 2253 "parsetl.cc" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -124;

  const signed char parser::yytable_ninf_ = -25;

  const short int
  parser::yypact_[] =
  {
     206,   400,   202,    25,   465,    11,   -48,   493,  -124,  -124,
    -124,   500,   526,   554,   582,   610,  -124,  -124,  -124,  -124,
    -124,  -124,   -14,  -124,   137,  -124,   438,  1279,  1279,  1279,
    1279,  1279,  1279,  1279,  1279,  1279,  1279,  1279,  1279,  1279,
    1279,  -124,  -124,  -124,  -124,  -124,    17,   617,  -124,   500,
    1278,  -124,  -124,  -124,    28,  -124,  -124,  -124,  -124,  -124,
     357,  -124,   694,  -124,   722,  -124,  -124,  -124,   462,  -124,
    -124,    -2,   414,    27,   309,  -124,  -124,  -124,  -124,  -124,
    -124,  -124,  -124,  -124,  -124,   643,   671,   699,   727,  -124,
     755,   783,   811,   839,   867,   895,   923,   951,   979,  1007,
    1035,  -124,  -124,  -124,  1279,  1279,  1279,  1279,  1279,  -124,
    -124,  -124,  1279,  1279,  1279,  1279,  1279,  -124,  -124,    19,
    1247,   334,  -124,   -22,   -13,     3,  -124,    -7,   -15,  -124,
    1042,  1068,  1094,  1120,  1146,  1172,  -124,  -124,   245,   280,
     446,  1198,  1224,  -124,  -124,  -124,  -124,  -124,  -124,    22,
     110,  -124,  -124,   750,   778,   806,   834,   862,   890,   918,
    -124,  -124,  -124,  -124,    33,  -124,  -124,  -124,  -124,    86,
    -124,  -124,  -124,  -124,  1387,  -124,  1387,  -124,  1387,  -124,
    1387,  -124,   256,  -124,  1396,  -124,    23,  -124,    23,  -124,
    1387,  -124,  1387,  -124,   228,  -124,   228,  -124,   228,  -124,
     228,  -124,   228,  -124,  -124,  -124,  -124,  -124,  -124,  -124,
    -124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,    84,
      34,    74,  -124,  -124,  -124,  -124,  1343,  -124,  1364,  -124,
    1354,  -124,  1354,  -124,  1334,  -124,  1334,   100,   102,  -124,
     107,   111,  -124,   129,  -124,    76,   101,   131,   122,  -124,
    1284,  -124,  1334,  -124,  -124,    50,  -124,  -124,  -124,   134,
    -124,   225,  -124,   146,  -124,   146,  -124,     4,  -124,     4,
    -124,  -124,  -124,  -124,  -124,  -124,   155,  -124,  -124,  -124,
    -124,  -124,  -124,  -124,  -124,   125,   164,   166,  -124,  -124,
    -124,  -124,  -124,   168,  -124,  -124,  -124
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,     0,     0,     0,     0,     0,     0,     0,    96,    95,
     170,     0,     0,     0,     0,     0,    54,    58,    59,    18,
       5,     3,    55,   127,   159,   128,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   186,   187,    17,    15,   171,     0,     0,    63,     0,
       0,    39,    40,    42,     0,    13,    11,    41,    78,    60,
       0,    62,     0,   104,     0,     9,     7,   103,     0,     1,
      19,     0,     0,     0,     0,   152,   151,   154,   153,   156,
     155,   158,   157,    57,    56,     0,     0,     0,     0,   161,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     2,     4,   172,     0,     0,     0,     0,     0,   178,
     179,   180,     0,     0,     0,     0,     0,    14,    16,     0,
       0,     0,    61,    25,     0,    20,    23,     0,     0,    43,
       0,     0,     0,     0,     0,     0,    46,    47,     0,     0,
       0,     0,     0,    10,    12,    82,    79,    80,    81,     0,
       0,   126,   125,     0,     0,     0,     0,     0,     0,     0,
       6,     8,    99,   102,     0,    97,   100,    91,    94,     0,
      89,   169,    92,   162,   160,   164,   163,   166,   165,   168,
     167,   136,   135,   138,   137,   130,   129,   132,   131,   140,
     139,   142,   141,   144,   143,   146,   145,   148,   147,   150,
     149,   134,   133,   173,   174,   175,   176,   177,   181,   182,
     183,   184,   185,    65,    67,    64,    66,    44,    30,    20,
       0,     0,    21,    29,    45,    73,    72,    84,    83,    69,
      68,    71,    70,    86,    85,    88,    87,    25,     0,    48,
      25,     0,    51,    25,    35,     0,    20,     0,     0,    75,
      74,    77,    76,   107,   110,     0,   105,   108,   118,   117,
     120,   119,   112,   111,   114,   113,   122,   121,   124,   123,
     116,   115,    98,   101,    90,    93,     0,    27,    28,    49,
      50,    52,    53,    37,    36,    20,     0,     0,    34,    38,
     106,   109,    26,     0,    32,    33,    31
  };

  const short int
  parser::yypgoto_[] =
  {
    -124,  -124,    94,   145,  -123,  -124,   -19,    39,  -124,  -124,
     -60,  -124,  -124,   187,     1,    66,    53,   220,   -49,    -5,
     201
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,     5,    20,    21,   126,   127,   128,   129,   145,    57,
      58,   147,   148,    22,    23,    60,    24,    25,    68,    26,
      46
  };

  const short int
  parser::yytable_[] =
  {
     146,   220,    72,    70,    59,    67,   162,    76,    78,    80,
      82,    69,    59,   150,   146,   152,   217,   247,     6,   153,
     154,   155,   156,   157,   158,   218,     6,   213,   219,   123,
     253,   223,    47,   159,    48,     9,   224,    49,    83,    84,
     167,   272,   221,   222,    96,    97,    98,    99,    59,   163,
      59,    59,   100,    50,    51,    52,    61,    53,   290,    54,
     146,   146,   146,    67,    61,    67,   -22,   124,   117,   125,
     214,    16,   277,   254,    17,    18,    19,    74,   168,   -24,
     174,   176,   178,   180,   273,   182,   184,   186,   188,   190,
     192,   194,   196,   198,   200,   202,    43,    55,    65,   274,
      61,   291,    61,    61,   259,   261,   263,   265,   267,   269,
     271,   255,   278,   120,   284,   121,   122,   285,   256,   238,
     241,   248,   286,   276,   222,   153,   154,   155,   156,   157,
     158,    59,    59,    59,    59,    59,    59,   275,   279,   159,
     287,   222,    59,    59,     7,   281,     8,    44,    56,    66,
     154,   155,   156,   280,    67,    67,    67,    67,    67,    67,
      67,   257,   282,   159,   293,   222,   146,   283,   146,   288,
     146,   102,   146,   289,   146,   159,   146,   239,   242,    85,
      86,    87,    88,    61,    61,    61,    61,    61,    61,    45,
     146,   118,   146,   292,    61,    61,   226,   228,   230,   232,
     234,   236,   294,     6,   295,   144,   296,   250,   252,     1,
       2,     3,     4,   161,    45,    45,    45,    45,    45,    45,
      45,    45,    45,    45,    45,    45,    45,    45,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   155,   156,    89,     0,   237,     0,    16,    96,
      97,    98,    99,    19,   159,     0,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    91,    92,    93,     0,     0,    96,    97,    98,
      99,   240,     0,   -22,   124,   100,   125,     0,     0,     0,
       0,    45,    45,    45,    45,    45,   -24,     0,     0,    45,
      45,    45,    45,    45,     0,   203,   204,   205,   206,   207,
     169,     0,     0,   208,   209,   210,   211,   212,   -22,   124,
       0,   125,   170,   171,   130,   131,   132,   133,   134,   135,
       0,   -24,     0,     0,     0,   169,     0,     0,    51,    52,
     136,    53,   137,    54,   138,   139,   140,   170,     0,   130,
     131,   132,   133,   134,   135,     0,   141,   142,     6,     0,
     172,     0,     0,    51,    52,   136,    53,   137,    54,   138,
     139,   140,   130,   131,   132,   133,   134,   135,     0,     0,
       0,   141,   142,     0,     0,   172,    51,    52,   136,    53,
     137,    54,   138,   139,   140,     0,     0,     0,     0,     0,
       0,     6,     0,     0,   141,   142,     0,     7,   143,     8,
       9,    10,    11,     0,     0,   164,     0,     0,     0,     0,
       0,     0,   165,     0,     0,    12,    13,    14,    15,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,     6,
       0,     0,     0,   100,     0,     0,    16,   243,     0,    17,
      18,    19,     0,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,     6,     0,   166,     6,   100,     0,     0,
       0,     0,    62,     0,    63,     0,     0,   153,   154,   155,
     156,   157,   158,     0,   244,   245,     0,   246,     0,   101,
       0,   159,     0,    64,    71,     0,     0,   -24,     0,     0,
       7,    73,     8,     9,    10,    11,     0,    47,     0,    48,
       9,    16,    49,   160,    17,    18,    19,     0,    12,    13,
      14,    15,     0,     0,     0,     0,     0,    75,    50,    51,
      52,     0,    53,     7,    54,     8,     9,    10,    11,    16,
       0,     0,    17,    18,     0,     0,    16,     0,     0,    17,
      18,    12,    13,    14,    15,    77,     0,     0,     0,     0,
       0,     7,     0,     8,     9,    10,    11,     0,     0,     0,
       0,     0,    16,     0,     0,    17,    18,     0,     0,    12,
      13,    14,    15,    79,     0,     0,     0,     0,     0,     7,
       0,     8,     9,    10,    11,     0,     0,     0,     0,     0,
      16,     0,     0,    17,    18,     0,     0,    12,    13,    14,
      15,    81,     0,     0,     0,     0,     0,     7,   119,     8,
       9,    10,    11,     0,    47,     0,    48,     9,    16,    49,
       0,    17,    18,     0,     0,    12,    13,    14,    15,     0,
       0,     0,     0,     0,   173,    50,    51,    52,     0,    53,
       7,    54,     8,     9,    10,    11,    16,     0,     0,    17,
      18,     0,     0,    16,     0,     0,    17,    18,    12,    13,
      14,    15,   175,     0,     0,     0,     0,     0,     7,     0,
       8,     9,    10,    11,     0,     0,     0,     0,     0,    16,
       0,     0,    17,    18,     0,   149,    12,    13,    14,    15,
     177,    62,     0,    63,     0,     0,     7,     0,     8,     9,
      10,    11,     0,     0,     0,     0,     0,    16,     0,     0,
      17,    18,    64,   151,    12,    13,    14,    15,   179,    62,
       0,    63,     0,     0,     7,     0,     8,     9,    10,    11,
      16,     0,     0,    17,    18,    16,     0,     0,    17,    18,
      64,   258,    12,    13,    14,    15,   181,    62,     0,    63,
       0,     0,     7,     0,     8,     9,    10,    11,    16,     0,
       0,    17,    18,    16,     0,     0,    17,    18,    64,   260,
      12,    13,    14,    15,   183,    62,     0,    63,     0,     0,
       7,     0,     8,     9,    10,    11,    16,     0,     0,    17,
      18,    16,     0,     0,    17,    18,    64,   262,    12,    13,
      14,    15,   185,    62,     0,    63,     0,     0,     7,     0,
       8,     9,    10,    11,    16,     0,     0,    17,    18,    16,
       0,     0,    17,    18,    64,   264,    12,    13,    14,    15,
     187,    62,     0,    63,     0,     0,     7,     0,     8,     9,
      10,    11,    16,     0,     0,    17,    18,    16,     0,     0,
      17,    18,    64,   266,    12,    13,    14,    15,   189,    62,
       0,    63,     0,     0,     7,     0,     8,     9,    10,    11,
      16,     0,     0,    17,    18,    16,     0,     0,    17,    18,
      64,   268,    12,    13,    14,    15,   191,    62,     0,    63,
       0,     0,     7,     0,     8,     9,    10,    11,    16,     0,
       0,    17,    18,    16,     0,     0,    17,    18,    64,   270,
      12,    13,    14,    15,   193,    62,     0,    63,     0,     0,
       7,     0,     8,     9,    10,    11,    16,     0,     0,    17,
      18,    16,     0,     0,    17,    18,    64,     0,    12,    13,
      14,    15,   195,     0,     0,     0,     0,     0,     7,     0,
       8,     9,    10,    11,    16,     0,     0,    17,    18,    16,
       0,     0,    17,    18,     0,     0,    12,    13,    14,    15,
     197,     0,     0,     0,     0,     0,     7,     0,     8,     9,
      10,    11,     0,     0,     0,     0,     0,    16,     0,     0,
      17,    18,     0,     0,    12,    13,    14,    15,   199,     0,
       0,     0,     0,     0,     7,     0,     8,     9,    10,    11,
       0,     0,     0,     0,     0,    16,     0,     0,    17,    18,
       0,     0,    12,    13,    14,    15,   201,     0,     0,     0,
       0,     0,     7,   225,     8,     9,    10,    11,     0,    47,
       0,    48,     9,    16,    49,     0,    17,    18,     0,     0,
      12,    13,    14,    15,     0,     0,     0,     0,     0,   227,
      50,    51,    52,     0,    53,    47,    54,    48,     9,     0,
      49,    16,     0,     0,    17,    18,     0,     0,    16,     0,
       0,    17,    18,     0,     0,   229,    50,    51,    52,     0,
      53,    47,    54,    48,     9,     0,    49,     0,     0,     0,
       0,     0,     0,     0,    16,     0,     0,    17,    18,     0,
       0,   231,    50,    51,    52,     0,    53,    47,    54,    48,
       9,     0,    49,     0,     0,     0,     0,     0,     0,     0,
      16,     0,     0,    17,    18,     0,     0,   233,    50,    51,
      52,     0,    53,    47,    54,    48,     9,     0,    49,     0,
       0,     0,     0,     0,     0,     0,    16,     0,     0,    17,
      18,     0,     0,   235,    50,    51,    52,     0,    53,    47,
      54,    48,     9,     0,    49,     0,     0,     0,     0,     0,
       0,     0,    16,     0,     0,    17,    18,     0,     0,   249,
      50,    51,    52,     0,    53,    47,    54,    48,     9,     0,
      49,     0,     0,     0,     0,     0,     0,     0,    16,     0,
       0,    17,    18,     0,     0,   251,    50,    51,    52,     0,
      53,    47,    54,    48,     9,     0,    49,     0,     0,     0,
       0,     0,     0,     0,    16,     0,     0,    17,    18,     0,
       0,     0,    50,    51,    52,   215,    53,     0,    54,     0,
       0,     0,   130,   131,   132,   133,   134,   135,     0,     0,
      16,     0,     0,    17,    18,     0,    51,    52,   136,    53,
     137,    54,   138,   139,   140,    47,     0,    48,     9,     0,
      49,     0,     0,     0,   141,   142,     0,     0,   216,   130,
     131,   132,   133,   134,   135,     0,    50,    51,    52,     0,
      53,     0,    54,    51,    52,   136,    53,   137,    54,   138,
     139,   140,     0,     0,    16,    16,     0,    17,    18,     0,
       0,     0,   142,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,   130,
     131,   132,   133,   134,   135,     0,     0,     0,     0,   131,
     132,   133,     0,    51,    52,   136,    53,   137,    54,   138,
     139,   140,    51,    52,   136,    53,   137,    54,   138,   139,
     140,   132,   133,    51,    52,   136,    53,   137,    54,   138,
     139,   140,     0,    51,    52,   136,    53,   137,    54,   138,
     139,   140,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,     0,    92,    93,     0,   100,    96,    97,    98,
      99,     0,     0,     0,     0,   100
  };

  const short int
  parser::yycheck_[] =
  {
      60,   124,     7,    51,     3,     4,     8,    12,    13,    14,
      15,     0,    11,    62,    74,    64,    38,   140,     1,    15,
      16,    17,    18,    19,    20,    38,     1,     8,    41,     1,
       8,    38,     7,    29,     9,    10,    51,    12,    52,    53,
      13,     8,    39,    40,    21,    22,    23,    24,    47,    51,
      49,    50,    29,    28,    29,    30,     3,    32,     8,    34,
     120,   121,   122,    62,    11,    64,    38,    39,    51,    41,
      51,    46,    38,    51,    49,    50,    51,    11,    51,    51,
      85,    86,    87,    88,    51,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,     2,     3,     4,    13,
      47,    51,    49,    50,   153,   154,   155,   156,   157,   158,
     159,     1,    38,    47,    38,    49,    50,    41,     8,   138,
     139,   140,   245,    39,    40,    15,    16,    17,    18,    19,
      20,   130,   131,   132,   133,   134,   135,    51,    38,    29,
      39,    40,   141,   142,     7,    38,     9,     2,     3,     4,
      16,    17,    18,    51,   153,   154,   155,   156,   157,   158,
     159,    51,    51,    29,    39,    40,   226,    38,   228,    38,
     230,    26,   232,    51,   234,    29,   236,   138,   139,    42,
      43,    44,    45,   130,   131,   132,   133,   134,   135,     2,
     250,    46,   252,    38,   141,   142,   130,   131,   132,   133,
     134,   135,    38,     1,    38,    60,    38,   141,   142,     3,
       4,     5,     6,    68,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    17,    18,    24,    -1,     1,    -1,    46,    21,
      22,    23,    24,    51,    29,    -1,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    16,    17,    18,    -1,    -1,    21,    22,    23,
      24,     1,    -1,    38,    39,    29,    41,    -1,    -1,    -1,
      -1,   104,   105,   106,   107,   108,    51,    -1,    -1,   112,
     113,   114,   115,   116,    -1,   104,   105,   106,   107,   108,
       1,    -1,    -1,   112,   113,   114,   115,   116,    38,    39,
      -1,    41,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    51,    -1,    -1,    -1,     1,    -1,    -1,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    13,    -1,    15,
      16,    17,    18,    19,    20,    -1,    47,    48,     1,    -1,
      51,    -1,    -1,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    47,    48,    -1,    -1,    51,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,    -1,    47,    48,    -1,     7,    51,     9,
      10,    11,    12,    -1,    -1,     1,    -1,    -1,    -1,    -1,
      -1,    -1,     8,    -1,    -1,    25,    26,    27,    28,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     1,
      -1,    -1,    -1,    29,    -1,    -1,    46,     1,    -1,    49,
      50,    51,    -1,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     1,    -1,    51,     1,    29,    -1,    -1,
      -1,    -1,     7,    -1,     9,    -1,    -1,    15,    16,    17,
      18,    19,    20,    -1,    38,    39,    -1,    41,    -1,    51,
      -1,    29,    -1,    28,     1,    -1,    -1,    51,    -1,    -1,
       7,     1,     9,    10,    11,    12,    -1,     7,    -1,     9,
      10,    46,    12,    51,    49,    50,    51,    -1,    25,    26,
      27,    28,    -1,    -1,    -1,    -1,    -1,     1,    28,    29,
      30,    -1,    32,     7,    34,     9,    10,    11,    12,    46,
      -1,    -1,    49,    50,    -1,    -1,    46,    -1,    -1,    49,
      50,    25,    26,    27,    28,     1,    -1,    -1,    -1,    -1,
      -1,     7,    -1,     9,    10,    11,    12,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    -1,    25,
      26,    27,    28,     1,    -1,    -1,    -1,    -1,    -1,     7,
      -1,     9,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    -1,    25,    26,    27,
      28,     1,    -1,    -1,    -1,    -1,    -1,     7,     1,     9,
      10,    11,    12,    -1,     7,    -1,     9,    10,    46,    12,
      -1,    49,    50,    -1,    -1,    25,    26,    27,    28,    -1,
      -1,    -1,    -1,    -1,     1,    28,    29,    30,    -1,    32,
       7,    34,     9,    10,    11,    12,    46,    -1,    -1,    49,
      50,    -1,    -1,    46,    -1,    -1,    49,    50,    25,    26,
      27,    28,     1,    -1,    -1,    -1,    -1,    -1,     7,    -1,
       9,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,     1,    25,    26,    27,    28,
       1,     7,    -1,     9,    -1,    -1,     7,    -1,     9,    10,
      11,    12,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      49,    50,    28,     1,    25,    26,    27,    28,     1,     7,
      -1,     9,    -1,    -1,     7,    -1,     9,    10,    11,    12,
      46,    -1,    -1,    49,    50,    46,    -1,    -1,    49,    50,
      28,     1,    25,    26,    27,    28,     1,     7,    -1,     9,
      -1,    -1,     7,    -1,     9,    10,    11,    12,    46,    -1,
      -1,    49,    50,    46,    -1,    -1,    49,    50,    28,     1,
      25,    26,    27,    28,     1,     7,    -1,     9,    -1,    -1,
       7,    -1,     9,    10,    11,    12,    46,    -1,    -1,    49,
      50,    46,    -1,    -1,    49,    50,    28,     1,    25,    26,
      27,    28,     1,     7,    -1,     9,    -1,    -1,     7,    -1,
       9,    10,    11,    12,    46,    -1,    -1,    49,    50,    46,
      -1,    -1,    49,    50,    28,     1,    25,    26,    27,    28,
       1,     7,    -1,     9,    -1,    -1,     7,    -1,     9,    10,
      11,    12,    46,    -1,    -1,    49,    50,    46,    -1,    -1,
      49,    50,    28,     1,    25,    26,    27,    28,     1,     7,
      -1,     9,    -1,    -1,     7,    -1,     9,    10,    11,    12,
      46,    -1,    -1,    49,    50,    46,    -1,    -1,    49,    50,
      28,     1,    25,    26,    27,    28,     1,     7,    -1,     9,
      -1,    -1,     7,    -1,     9,    10,    11,    12,    46,    -1,
      -1,    49,    50,    46,    -1,    -1,    49,    50,    28,     1,
      25,    26,    27,    28,     1,     7,    -1,     9,    -1,    -1,
       7,    -1,     9,    10,    11,    12,    46,    -1,    -1,    49,
      50,    46,    -1,    -1,    49,    50,    28,    -1,    25,    26,
      27,    28,     1,    -1,    -1,    -1,    -1,    -1,     7,    -1,
       9,    10,    11,    12,    46,    -1,    -1,    49,    50,    46,
      -1,    -1,    49,    50,    -1,    -1,    25,    26,    27,    28,
       1,    -1,    -1,    -1,    -1,    -1,     7,    -1,     9,    10,
      11,    12,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    -1,    25,    26,    27,    28,     1,    -1,
      -1,    -1,    -1,    -1,     7,    -1,     9,    10,    11,    12,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    49,    50,
      -1,    -1,    25,    26,    27,    28,     1,    -1,    -1,    -1,
      -1,    -1,     7,     1,     9,    10,    11,    12,    -1,     7,
      -1,     9,    10,    46,    12,    -1,    49,    50,    -1,    -1,
      25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,     1,
      28,    29,    30,    -1,    32,     7,    34,     9,    10,    -1,
      12,    46,    -1,    -1,    49,    50,    -1,    -1,    46,    -1,
      -1,    49,    50,    -1,    -1,     1,    28,    29,    30,    -1,
      32,     7,    34,     9,    10,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,
      -1,     1,    28,    29,    30,    -1,    32,     7,    34,     9,
      10,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    -1,     1,    28,    29,
      30,    -1,    32,     7,    34,     9,    10,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    49,
      50,    -1,    -1,     1,    28,    29,    30,    -1,    32,     7,
      34,     9,    10,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    -1,     1,
      28,    29,    30,    -1,    32,     7,    34,     9,    10,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    49,    50,    -1,    -1,     1,    28,    29,    30,    -1,
      32,     7,    34,     9,    10,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,
      -1,    -1,    28,    29,    30,     8,    32,    -1,    34,    -1,
      -1,    -1,    15,    16,    17,    18,    19,    20,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    29,    30,    31,    32,
      33,    34,    35,    36,    37,     7,    -1,     9,    10,    -1,
      12,    -1,    -1,    -1,    47,    48,    -1,    -1,    51,    15,
      16,    17,    18,    19,    20,    -1,    28,    29,    30,    -1,
      32,    -1,    34,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    -1,    -1,    46,    46,    -1,    49,    50,    -1,
      -1,    -1,    48,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    16,
      17,    18,    -1,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    17,    18,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    -1,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    -1,    17,    18,    -1,    29,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    29
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     3,     4,     5,     6,    71,     1,     7,     9,    10,
      11,    12,    25,    26,    27,    28,    46,    49,    50,    51,
      72,    73,    83,    84,    86,    87,    89,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    72,    73,    83,    90,     7,     9,    12,
      28,    29,    30,    32,    34,    72,    73,    79,    80,    84,
      85,    86,     7,     9,    28,    72,    73,    84,    88,     0,
      51,     1,    89,     1,    85,     1,    89,     1,    89,     1,
      89,     1,    89,    52,    53,    42,    43,    44,    45,    87,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      29,    51,    73,    90,    90,    90,    90,    90,    90,    90,
      90,    90,    90,    90,    90,    90,    90,    51,    73,     1,
      85,    85,    85,     1,    39,    41,    74,    75,    76,    77,
      15,    16,    17,    18,    19,    20,    31,    33,    35,    36,
      37,    47,    48,    51,    73,    78,    80,    81,    82,     1,
      88,     1,    88,    15,    16,    17,    18,    19,    20,    29,
      51,    73,     8,    51,     1,     8,    51,    13,    51,     1,
      13,    14,    51,     1,    89,     1,    89,     1,    89,     1,
      89,     1,    89,     1,    89,     1,    89,     1,    89,     1,
      89,     1,    89,     1,    89,     1,    89,     1,    89,     1,
      89,     1,    89,    90,    90,    90,    90,    90,    90,    90,
      90,    90,    90,     8,    51,     8,    51,    38,    38,    41,
      74,    39,    40,    38,    51,     1,    85,     1,    85,     1,
      85,     1,    85,     1,    85,     1,    85,     1,    76,    77,
       1,    76,    77,     1,    38,    39,    41,    74,    76,     1,
      85,     1,    85,     8,    51,     1,     8,    51,     1,    88,
       1,    88,     1,    88,     1,    88,     1,    88,     1,    88,
       1,    88,     8,    51,    13,    51,    39,    38,    38,    38,
      51,    38,    51,    38,    38,    41,    74,    39,    38,    51,
       8,    51,    38,    39,    38,    38,    38
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    70,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    72,    73,
      74,    74,    75,    75,    76,    76,    77,    77,    77,    77,
      77,    78,    78,    78,    78,    78,    78,    78,    78,    79,
      79,    80,    80,    80,    80,    80,    81,    81,    81,    81,
      81,    82,    82,    82,    83,    84,    84,    84,    84,    84,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    86,
      86,    86,    86,    86,    86,    86,    87,    87,    87,    87,
      87,    87,    87,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      90,    90,    90,    90,    90,    90,    90,    90
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     3,     2,     3,     2,     3,     2,     3,     2,
       3,     2,     3,     2,     3,     2,     3,     2,     1,     2,
       1,     2,     0,     1,     0,     1,     4,     3,     3,     2,
       2,     5,     4,     4,     3,     2,     3,     3,     3,     1,
       1,     1,     1,     2,     3,     3,     1,     1,     2,     3,
       3,     2,     3,     3,     1,     1,     2,     2,     1,     1,
       1,     2,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       4,     3,     3,     4,     3,     1,     1,     3,     4,     3,
       3,     4,     3,     1,     1,     3,     4,     3,     3,     4,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       3,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     2,     3,     3,     3,     3,     3,     2,     2,
       2,     3,     3,     3,     3,     3,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "$end", "error", "$undefined", "\"LTL start marker\"",
  "\"LBT start marker\"", "\"SERE start marker\"",
  "\"BOOLEAN start marker\"", "\"opening parenthesis\"",
  "\"closing parenthesis\"", "\"(...) block\"", "\"{...} block\"",
  "\"{...}! block\"", "\"opening brace\"", "\"closing brace\"",
  "\"closing brace-bang\"", "\"or operator\"", "\"xor operator\"",
  "\"and operator\"", "\"short and operator\"", "\"implication operator\"",
  "\"equivalent operator\"", "\"until operator\"", "\"release operator\"",
  "\"weak until operator\"", "\"strong release operator\"",
  "\"sometimes operator\"", "\"always operator\"", "\"next operator\"",
  "\"not operator\"", "\"star operator\"", "\"bracket star operator\"",
  "\"bracket fusion-star operator\"", "\"plus operator\"",
  "\"fusion-plus operator\"", "\"opening bracket for star operator\"",
  "\"opening bracket for fusion-star operator\"",
  "\"opening bracket for equal operator\"",
  "\"opening bracket for goto operator\"", "\"closing bracket\"",
  "\"number for square bracket operator\"", "\"unbounded mark\"",
  "\"separator for square bracket operator\"",
  "\"universal concat operator\"", "\"existential concat operator\"",
  "\"universal non-overlapping concat operator\"",
  "\"existential non-overlapping concat operator\"",
  "\"atomic proposition\"", "\"concat operator\"", "\"fusion operator\"",
  "\"constant true\"", "\"constant false\"", "\"end of formula\"",
  "\"negative suffix\"", "\"positive suffix\"", "'!'", "'&'", "'|'", "'^'",
  "'i'", "'e'", "'X'", "'F'", "'G'", "'U'", "'V'", "'R'", "'W'", "'M'",
  "'t'", "'f'", "$accept", "result", "emptyinput", "enderror",
  "OP_SQBKT_SEP_unbounded", "OP_SQBKT_SEP_opt", "error_opt",
  "sqbracketargs", "gotoargs", "kleen_star", "starargs", "fstarargs",
  "equalargs", "atomprop", "booleanatom", "sere", "bracedsere",
  "parenthesedsubformula", "boolformula", "subformula", "lbtformula", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   279,   279,   284,   289,   294,   296,   301,   306,   311,
     313,   318,   323,   328,   330,   335,   340,   345,   348,   354,
     360,   360,   361,   361,   362,   362,   365,   367,   369,   371,
     373,   377,   379,   381,   383,   385,   387,   389,   392,   397,
     397,   399,   401,   403,   405,   408,   412,   414,   416,   418,
     422,   427,   429,   432,   438,   446,   447,   448,   452,   454,
     457,   458,   470,   471,   480,   482,   488,   492,   498,   500,
     503,   505,   508,   510,   512,   514,   516,   518,   520,   529,
     538,   547,   565,   583,   597,   599,   612,   614,   628,   631,
     633,   637,   642,   647,   652,   658,   668,   676,   678,   682,
     687,   691,   696,   704,   705,   714,   716,   720,   725,   730,
     735,   741,   743,   745,   747,   749,   751,   753,   755,   757,
     759,   761,   763,   765,   767,   769,   771,   774,   775,   776,
     778,   780,   782,   784,   786,   788,   790,   792,   794,   796,
     798,   800,   802,   804,   806,   808,   810,   812,   814,   816,
     818,   820,   822,   824,   826,   828,   830,   832,   834,   836,
     838,   840,   842,   845,   847,   851,   856,   860,   865,   869,
     872,   883,   884,   886,   888,   890,   892,   894,   896,   898,
     900,   902,   904,   906,   908,   910,   912,   914
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  // Symbol number corresponding to token number t.
  inline
  parser::token_number_type
  parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    54,     2,     2,     2,     2,    55,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      61,    62,     2,     2,     2,     2,     2,    67,     2,     2,
       2,     2,    65,     2,     2,    63,    64,    66,    60,     2,
       2,     2,     2,     2,    57,     2,     2,     2,     2,     2,
       2,    59,    69,     2,     2,    58,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    68,     2,     2,     2,
       2,     2,     2,     2,    56,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53
    };
    const unsigned int user_token_number_max_ = 308;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }


} // tlyy
#line 3124 "parsetl.cc" // lalr1.cc:1167
#line 918 "parsetl.yy" // lalr1.cc:1168


void
tlyy::parser::error(const location_type& location, const std::string& message)
{
  error_list.emplace_back(location, message);
}

namespace spot
{
  parsed_formula
  parse_infix_psl(const std::string& ltl_string,
		  environment& env,
		  bool debug, bool lenient)
  {
    parsed_formula result(ltl_string);
    flex_set_buffer(ltl_string,
		    tlyy::parser::token::START_LTL,
		    lenient);
    tlyy::parser parser(result.errors, env, result.f);
    parser.set_debug_level(debug);
    parser.parse();
    flex_unset_buffer();
    return result;
  }

  parsed_formula
  parse_infix_boolean(const std::string& ltl_string,
		      environment& env,
		      bool debug, bool lenient)
  {
    parsed_formula result(ltl_string);
    flex_set_buffer(ltl_string,
		    tlyy::parser::token::START_BOOL,
		    lenient);
    tlyy::parser parser(result.errors, env, result.f);
    parser.set_debug_level(debug);
    parser.parse();
    flex_unset_buffer();
    return result;
  }

  parsed_formula
  parse_prefix_ltl(const std::string& ltl_string,
		   environment& env,
		   bool debug)
  {
    parsed_formula result(ltl_string);
    flex_set_buffer(ltl_string,
		    tlyy::parser::token::START_LBT,
		    false);
    tlyy::parser parser(result.errors, env, result.f);
    parser.set_debug_level(debug);
    parser.parse();
    flex_unset_buffer();
    return result;
  }

  parsed_formula
  parse_infix_sere(const std::string& sere_string,
		   environment& env,
		   bool debug,
		   bool lenient)
  {
    parsed_formula result(sere_string);
    flex_set_buffer(sere_string,
		    tlyy::parser::token::START_SERE,
		    lenient);
    tlyy::parser parser(result.errors, env, result.f);
    parser.set_debug_level(debug);
    parser.parse();
    flex_unset_buffer();
    return result;
  }

  formula
  parse_formula(const std::string& ltl_string, environment& env)
  {
    parsed_formula pf = parse_infix_psl(ltl_string, env);
    std::ostringstream s;
    if (pf.format_errors(s))
      {
	parsed_formula pg = parse_prefix_ltl(ltl_string, env);
	if (pg.errors.empty())
	  return pg.f;
	else
	  throw parse_error(s.str());
      }
    return pf.f;
  }
}

// Local Variables:
// mode: c++
// End:
