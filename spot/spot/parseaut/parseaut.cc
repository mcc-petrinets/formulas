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
#define yylex   hoayylex

// First part of user declarations.

#line 39 "parseaut.cc" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "parseaut.hh"

// User implementation prologue.

#line 53 "parseaut.cc" // lalr1.cc:412
// Unqualified %code blocks.
#line 199 "parseaut.yy" // lalr1.cc:413

#include <sstream>

  /* parseaut.hh and parsedecl.hh include each other recursively.
   We must ensure that YYSTYPE is declared (by the above %union)
   before parsedecl.hh uses it. */
#include <spot/parseaut/parsedecl.hh>

  static void fill_guards(result_& res);

#line 66 "parseaut.cc" // lalr1.cc:413


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


namespace hoayy {
#line 152 "parseaut.cc" // lalr1.cc:479

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
  parser::parser (void* scanner_yyarg, result_& res_yyarg, spot::location initial_loc_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      res (res_yyarg),
      initial_loc (initial_loc_yyarg)
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
            case 18: // "identifier"

#line 287 "parseaut.yy" // lalr1.cc:614
        { delete (yysym.value.str); }
#line 407 "parseaut.cc" // lalr1.cc:614
        break;

      case 19: // "header name"

#line 287 "parseaut.yy" // lalr1.cc:614
        { delete (yysym.value.str); }
#line 414 "parseaut.cc" // lalr1.cc:614
        break;

      case 20: // "alias name"

#line 287 "parseaut.yy" // lalr1.cc:614
        { delete (yysym.value.str); }
#line 421 "parseaut.cc" // lalr1.cc:614
        break;

      case 21: // "string"

#line 287 "parseaut.yy" // lalr1.cc:614
        { delete (yysym.value.str); }
#line 428 "parseaut.cc" // lalr1.cc:614
        break;

      case 44: // "boolean formula"

#line 287 "parseaut.yy" // lalr1.cc:614
        { delete (yysym.value.str); }
#line 435 "parseaut.cc" // lalr1.cc:614
        break;

      case 69: // string_opt

#line 287 "parseaut.yy" // lalr1.cc:614
        { delete (yysym.value.str); }
#line 442 "parseaut.cc" // lalr1.cc:614
        break;

      case 89: // state-conj-2

#line 291 "parseaut.yy" // lalr1.cc:614
        { delete (yysym.value.states); }
#line 449 "parseaut.cc" // lalr1.cc:614
        break;

      case 90: // init-state-conj-2

#line 291 "parseaut.yy" // lalr1.cc:614
        { delete (yysym.value.states); }
#line 456 "parseaut.cc" // lalr1.cc:614
        break;

      case 91: // label-expr

#line 288 "parseaut.yy" // lalr1.cc:614
        { bdd_delref((yysym.value.b)); }
#line 463 "parseaut.cc" // lalr1.cc:614
        break;

      case 93: // acceptance-cond

#line 290 "parseaut.yy" // lalr1.cc:614
        { delete (yysym.value.code); }
#line 470 "parseaut.cc" // lalr1.cc:614
        break;

      case 111: // state-conj-checked

#line 291 "parseaut.yy" // lalr1.cc:614
        { delete (yysym.value.states); }
#line 477 "parseaut.cc" // lalr1.cc:614
        break;

      case 128: // nc-one-ident

#line 287 "parseaut.yy" // lalr1.cc:614
        { delete (yysym.value.str); }
#line 484 "parseaut.cc" // lalr1.cc:614
        break;

      case 129: // nc-ident-list

#line 287 "parseaut.yy" // lalr1.cc:614
        { delete (yysym.value.str); }
#line 491 "parseaut.cc" // lalr1.cc:614
        break;

      case 130: // nc-transition-block

#line 306 "parseaut.yy" // lalr1.cc:614
        {
  for (auto i = (yysym.value.list)->begin(); i != (yysym.value.list)->end(); ++i)
  {
    bdd_delref(i->first);
    delete i->second;
  }
  delete (yysym.value.list);
  }
#line 505 "parseaut.cc" // lalr1.cc:614
        break;

      case 132: // nc-transitions

#line 306 "parseaut.yy" // lalr1.cc:614
        {
  for (auto i = (yysym.value.list)->begin(); i != (yysym.value.list)->end(); ++i)
  {
    bdd_delref(i->first);
    delete i->second;
  }
  delete (yysym.value.list);
  }
#line 519 "parseaut.cc" // lalr1.cc:614
        break;

      case 133: // nc-formula-or-ident

#line 287 "parseaut.yy" // lalr1.cc:614
        { delete (yysym.value.str); }
#line 526 "parseaut.cc" // lalr1.cc:614
        break;

      case 134: // nc-formula

#line 288 "parseaut.yy" // lalr1.cc:614
        { bdd_delref((yysym.value.b)); }
#line 533 "parseaut.cc" // lalr1.cc:614
        break;

      case 135: // nc-opt-dest

#line 287 "parseaut.yy" // lalr1.cc:614
        { delete (yysym.value.str); }
#line 540 "parseaut.cc" // lalr1.cc:614
        break;

      case 136: // nc-src-dest

#line 289 "parseaut.yy" // lalr1.cc:614
        { bdd_delref((yysym.value.p)->first); delete (yysym.value.p)->second; delete (yysym.value.p); }
#line 547 "parseaut.cc" // lalr1.cc:614
        break;

      case 137: // nc-transition

#line 289 "parseaut.yy" // lalr1.cc:614
        { bdd_delref((yysym.value.p)->first); delete (yysym.value.p)->second; delete (yysym.value.p); }
#line 554 "parseaut.cc" // lalr1.cc:614
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
            case 18: // "identifier"

#line 314 "parseaut.yy" // lalr1.cc:636
        {
    if ((yysym.value.str))
      debug_stream() << *(yysym.value.str);
    else
      debug_stream() << "\"\""; }
#line 589 "parseaut.cc" // lalr1.cc:636
        break;

      case 19: // "header name"

#line 314 "parseaut.yy" // lalr1.cc:636
        {
    if ((yysym.value.str))
      debug_stream() << *(yysym.value.str);
    else
      debug_stream() << "\"\""; }
#line 600 "parseaut.cc" // lalr1.cc:636
        break;

      case 20: // "alias name"

#line 314 "parseaut.yy" // lalr1.cc:636
        {
    if ((yysym.value.str))
      debug_stream() << *(yysym.value.str);
    else
      debug_stream() << "\"\""; }
#line 611 "parseaut.cc" // lalr1.cc:636
        break;

      case 21: // "string"

#line 314 "parseaut.yy" // lalr1.cc:636
        {
    if ((yysym.value.str))
      debug_stream() << *(yysym.value.str);
    else
      debug_stream() << "\"\""; }
#line 622 "parseaut.cc" // lalr1.cc:636
        break;

      case 22: // "integer"

#line 319 "parseaut.yy" // lalr1.cc:636
        { debug_stream() << (yysym.value.num); }
#line 629 "parseaut.cc" // lalr1.cc:636
        break;

      case 44: // "boolean formula"

#line 314 "parseaut.yy" // lalr1.cc:636
        {
    if ((yysym.value.str))
      debug_stream() << *(yysym.value.str);
    else
      debug_stream() << "\"\""; }
#line 640 "parseaut.cc" // lalr1.cc:636
        break;

      case 47: // "LBTT header"

#line 319 "parseaut.yy" // lalr1.cc:636
        { debug_stream() << (yysym.value.num); }
#line 647 "parseaut.cc" // lalr1.cc:636
        break;

      case 48: // "state acceptance"

#line 319 "parseaut.yy" // lalr1.cc:636
        { debug_stream() << (yysym.value.num); }
#line 654 "parseaut.cc" // lalr1.cc:636
        break;

      case 49: // "acceptance sets for empty automaton"

#line 319 "parseaut.yy" // lalr1.cc:636
        { debug_stream() << (yysym.value.num); }
#line 661 "parseaut.cc" // lalr1.cc:636
        break;

      case 50: // "acceptance set"

#line 319 "parseaut.yy" // lalr1.cc:636
        { debug_stream() << (yysym.value.num); }
#line 668 "parseaut.cc" // lalr1.cc:636
        break;

      case 51: // "state number"

#line 319 "parseaut.yy" // lalr1.cc:636
        { debug_stream() << (yysym.value.num); }
#line 675 "parseaut.cc" // lalr1.cc:636
        break;

      case 52: // "destination number"

#line 319 "parseaut.yy" // lalr1.cc:636
        { debug_stream() << (yysym.value.num); }
#line 682 "parseaut.cc" // lalr1.cc:636
        break;

      case 69: // string_opt

#line 314 "parseaut.yy" // lalr1.cc:636
        {
    if ((yysym.value.str))
      debug_stream() << *(yysym.value.str);
    else
      debug_stream() << "\"\""; }
#line 693 "parseaut.cc" // lalr1.cc:636
        break;

      case 89: // state-conj-2

#line 292 "parseaut.yy" // lalr1.cc:636
        {
  auto& os = debug_stream();
  os << '{';
  bool notfirst = false;
  for (auto i: *(yysym.value.states))
  {
    if (notfirst)
      os << ", ";
    else
      notfirst = true;
    os << i;
  }
  os << '}';
}
#line 713 "parseaut.cc" // lalr1.cc:636
        break;

      case 90: // init-state-conj-2

#line 292 "parseaut.yy" // lalr1.cc:636
        {
  auto& os = debug_stream();
  os << '{';
  bool notfirst = false;
  for (auto i: *(yysym.value.states))
  {
    if (notfirst)
      os << ", ";
    else
      notfirst = true;
    os << i;
  }
  os << '}';
}
#line 733 "parseaut.cc" // lalr1.cc:636
        break;

      case 92: // acc-set

#line 319 "parseaut.yy" // lalr1.cc:636
        { debug_stream() << (yysym.value.num); }
#line 740 "parseaut.cc" // lalr1.cc:636
        break;

      case 95: // state-num

#line 319 "parseaut.yy" // lalr1.cc:636
        { debug_stream() << (yysym.value.num); }
#line 747 "parseaut.cc" // lalr1.cc:636
        break;

      case 96: // checked-state-num

#line 319 "parseaut.yy" // lalr1.cc:636
        { debug_stream() << (yysym.value.num); }
#line 754 "parseaut.cc" // lalr1.cc:636
        break;

      case 111: // state-conj-checked

#line 292 "parseaut.yy" // lalr1.cc:636
        {
  auto& os = debug_stream();
  os << '{';
  bool notfirst = false;
  for (auto i: *(yysym.value.states))
  {
    if (notfirst)
      os << ", ";
    else
      notfirst = true;
    os << i;
  }
  os << '}';
}
#line 774 "parseaut.cc" // lalr1.cc:636
        break;

      case 120: // sign

#line 319 "parseaut.yy" // lalr1.cc:636
        { debug_stream() << (yysym.value.num); }
#line 781 "parseaut.cc" // lalr1.cc:636
        break;

      case 128: // nc-one-ident

#line 314 "parseaut.yy" // lalr1.cc:636
        {
    if ((yysym.value.str))
      debug_stream() << *(yysym.value.str);
    else
      debug_stream() << "\"\""; }
#line 792 "parseaut.cc" // lalr1.cc:636
        break;

      case 129: // nc-ident-list

#line 314 "parseaut.yy" // lalr1.cc:636
        {
    if ((yysym.value.str))
      debug_stream() << *(yysym.value.str);
    else
      debug_stream() << "\"\""; }
#line 803 "parseaut.cc" // lalr1.cc:636
        break;

      case 133: // nc-formula-or-ident

#line 314 "parseaut.yy" // lalr1.cc:636
        {
    if ((yysym.value.str))
      debug_stream() << *(yysym.value.str);
    else
      debug_stream() << "\"\""; }
#line 814 "parseaut.cc" // lalr1.cc:636
        break;

      case 135: // nc-opt-dest

#line 314 "parseaut.yy" // lalr1.cc:636
        {
    if ((yysym.value.str))
      debug_stream() << *(yysym.value.str);
    else
      debug_stream() << "\"\""; }
#line 825 "parseaut.cc" // lalr1.cc:636
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


    // User initialization code.
    #line 184 "parseaut.yy" // lalr1.cc:741
{ yyla.location = res.h->loc = initial_loc; }

#line 941 "parseaut.cc" // lalr1.cc:741

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
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location, scanner, PARSE_ERROR_LIST));
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
#line 322 "parseaut.yy" // lalr1.cc:859
    { res.h->loc = yylhs.location; YYACCEPT; }
#line 1051 "parseaut.cc" // lalr1.cc:859
    break;

  case 3:
#line 323 "parseaut.yy" // lalr1.cc:859
    { YYABORT; }
#line 1057 "parseaut.cc" // lalr1.cc:859
    break;

  case 4:
#line 324 "parseaut.yy" // lalr1.cc:859
    { YYABORT; }
#line 1063 "parseaut.cc" // lalr1.cc:859
    break;

  case 5:
#line 326 "parseaut.yy" // lalr1.cc:859
    {
       error(yystack_[1].location, "leading garbage was ignored");
       res.h->loc = yystack_[0].location;
       YYACCEPT;
     }
#line 1073 "parseaut.cc" // lalr1.cc:859
    break;

  case 6:
#line 332 "parseaut.yy" // lalr1.cc:859
    { res.h->type = spot::parsed_aut_type::HOA; }
#line 1079 "parseaut.cc" // lalr1.cc:859
    break;

  case 7:
#line 333 "parseaut.yy" // lalr1.cc:859
    { res.h->type = spot::parsed_aut_type::NeverClaim; }
#line 1085 "parseaut.cc" // lalr1.cc:859
    break;

  case 8:
#line 334 "parseaut.yy" // lalr1.cc:859
    { res.h->type = spot::parsed_aut_type::LBTT; }
#line 1091 "parseaut.cc" // lalr1.cc:859
    break;

  case 12:
#line 344 "parseaut.yy" // lalr1.cc:859
    { (yylhs.value.str) = nullptr; }
#line 1097 "parseaut.cc" // lalr1.cc:859
    break;

  case 13:
#line 345 "parseaut.yy" // lalr1.cc:859
    { (yylhs.value.str) = (yystack_[0].value.str); }
#line 1103 "parseaut.cc" // lalr1.cc:859
    break;

  case 16:
#line 349 "parseaut.yy" // lalr1.cc:859
    {
          bool v1plus = strverscmp("v1", res.format_version.c_str()) < 0;
	  // Preallocate the states if we know their number.
	  if (res.states >= 0)
	    {
	      unsigned states = res.states;
	      for (auto& p : res.start)
                for (unsigned s: p.second)
                  if ((unsigned) res.states <= s)
                    {
                      error(p.first, "initial state number is larger "
                            "than state count...");
                      error(res.states_loc, "... declared here.");
                      states = std::max(states, s + 1);
                    }
	      if (res.opts.want_kripke)
		res.h->ks->new_states(states, bddfalse);
	      else
		res.h->aut->new_states(states);
	      res.info_states.resize(states);
	    }
	  if (res.accset < 0)
	    {
	      error(yylhs.location, "missing 'Acceptance:' header");
	      res.ignore_acc = true;
	    }
	  // Process properties.
	  {
	    auto explicit_labels = res.prop_is_true("explicit-labels");
	    auto implicit_labels = res.prop_is_true("implicit-labels");

	    if (implicit_labels)
	      {
		if (res.opts.want_kripke)
		  error(implicit_labels.loc,
			"Kripke structure may not use implicit labels");

		if (explicit_labels)
		  {
		    error(implicit_labels.loc,
			  "'properties: implicit-labels' is incompatible "
			  "with...");
		    error(explicit_labels.loc,
			  "... 'properties: explicit-labels'.");
		  }
		else
		  {
		    res.label_style = Implicit_Labels;
		  }
	      }

	    auto trans_labels = res.prop_is_true("trans-labels");
	    auto state_labels = res.prop_is_true("state-labels");

	    if (trans_labels)
	      {
		if (res.opts.want_kripke)
		  error(trans_labels.loc,
			"Kripke structures may not use transition labels");

		if (state_labels)
		  {
		    error(trans_labels.loc,
			  "'properties: trans-labels' is incompatible with...");
		    error(state_labels.loc,
			  "... 'properties: state-labels'.");
		  }
		else
		  {
		    if (res.label_style != Implicit_Labels)
		      res.label_style = Trans_Labels;
		  }
	      }
	    else if (state_labels)
	      {
		if (res.label_style != Implicit_Labels)
		  {
		    res.label_style = State_Labels;
		  }
		else
		  {
		    error(state_labels.loc,
			  "'properties: state-labels' is incompatible with...");
		    error(implicit_labels.loc,
			  "... 'properties: implicit-labels'.");
		  }
	      }

	    if (res.opts.want_kripke && res.label_style != State_Labels)
	      error(yylhs.location,
		    "Kripke structures should use 'properties: state-labels'");

	    auto state_acc = res.prop_is_true("state-acc");
	    auto trans_acc = res.prop_is_true("trans-acc");
	    if (trans_acc)
	      {
		if (state_acc)
		  {
		    error(trans_acc.loc,
			  "'properties: trans-acc' is incompatible with...");
		    error(state_acc.loc,
			  "... 'properties: state-acc'.");
		  }
		else
		  {
		    res.acc_style = Trans_Acc;
		  }
	      }
	    else if (state_acc)
	      {
		res.acc_style = State_Acc;
	      }

            if (auto univ_branch = res.prop_is_true("univ-branch"))
              if (res.opts.want_kripke)
                error(univ_branch.loc,
                    "Kripke structures may not use 'properties: univ-branch'");
          }
	  {
	    unsigned ss = res.start.size();
	    auto det = res.prop_is_true("deterministic");
	    auto no_exist = res.prop_is_false("exist-branch");
	    if (ss > 1)
	      {
		if (det)
		  {
		    error(det.loc,
			  "deterministic automata should have at most "
			  "one initial state");
                    res.universal = spot::trival::maybe();
		  }
                else if (no_exist)
                  {
		    error(no_exist.loc,
			  "universal automata should have at most "
			  "one initial state");
                    res.universal = spot::trival::maybe();
                  }
	      }
	    else
	      {
		// Assume the automaton is deterministic until proven
		// wrong, or unless we are building a Kripke structure.
                if (!res.opts.want_kripke)
                  {
                    res.universal = true;
                    res.existential = true;
                  }
	      }
            for (auto& ss: res.start)
              {
                if (ss.second.size() > 1)
                  {
                    if (auto no_univ = res.prop_is_false("univ-branch"))
                      {
                        error(ss.first,
                              "conjunct initial state despite...");
                        error(no_univ.loc, "... property: !univ-branch");
                      }
                    else if (v1plus)
                      if (auto det = res.prop_is_true("deterministic"))
                        {
                          error(ss.first,
                                "conjunct initial state despite...");
                          error(det.loc, "... property: deterministic");
                        }
                    res.existential = false;
                  }
              }
	    auto complete = res.prop_is_true("complete");
	    if (ss < 1)
	      {
		if (complete)
		  {
		    error(complete.loc,
			  "complete automata should have at least "
			  "one initial state");
		  }
		res.complete = false;
	      }
	    else
	      {
		// Assume the automaton is complete until proven
		// wrong.
                if (!res.opts.want_kripke)
                  res.complete = true;
	      }
	    // if ap_count == 0, then a Kripke structure could be
	    // declared complete, although that probably doesn't
	    // matter.
	    if (res.opts.want_kripke && complete && res.ap_count > 0)
	      error(complete.loc,
		    "Kripke structure may not be complete");
	  }
	  if (res.opts.trust_hoa)
	    {
	      auto& a = res.aut_or_ks;
	      auto& p = res.props;
	      auto e = p.end();
	      auto si = p.find("stutter-invariant");
	      if (si != e)
		{
		  a->prop_stutter_invariant(si->second.val);
		  auto i = p.find("stutter-sensitive");
		  if (i != e && si->second.val == i->second.val)
		    error(i->second.loc,
			  "automaton cannot be both stutter-invariant"
			  "and stutter-sensitive");
		}
	      else
		{
		  auto ss = p.find("stutter-sensitive");
		  if (ss != e)
		    a->prop_stutter_invariant(!ss->second.val);
		}
	      auto iw = p.find("inherently-weak");
	      auto vw = p.find("very-weak");
	      auto w = p.find("weak");
	      auto t = p.find("terminal");
              if (vw != e)
                {
                  a->prop_very_weak(vw->second.val);
                  if (w != e && !w->second.val && vw->second.val)
                    {
		      error(w->second.loc,
                            "'properties: !weak' contradicts...");
		      error(vw->second.loc,
			    "... 'properties: very-weak' given here");
                    }
                  if (iw != e && !iw->second.val && vw->second.val)
                    {
		      error(iw->second.loc,
                            "'properties: !inherently-weak' contradicts...");
		      error(vw->second.loc,
			    "... 'properties: very-weak' given here");
                    }
                }
	      if (iw != e)
		{
		  a->prop_inherently_weak(iw->second.val);
		  if (w != e && !iw->second.val && w->second.val)
		    {
		      error(w->second.loc, "'properties: weak' contradicts...");
		      error(iw->second.loc,
			    "... 'properties: !inherently-weak' given here");
		    }
		  if (t != e && !iw->second.val && t->second.val)
		    {
		      error(t->second.loc,
			    "'properties: terminal' contradicts...");
		      error(iw->second.loc,
			    "... 'properties: !inherently-weak' given here");
		    }
		}
	      if (w != e)
		{
		  a->prop_weak(w->second.val);
		  if (t != e && !w->second.val && t->second.val)
		    {
		      error(t->second.loc,
			    "'properties: terminal' contradicts...");
		      error(w->second.loc,
			    "... 'properties: !weak' given here");
		    }
		}
	      if (t != e)
		a->prop_terminal(t->second.val);
	      auto u = p.find("unambiguous");
	      if (u != e)
		{
		  a->prop_unambiguous(u->second.val);
		  auto d = p.find("deterministic");
		  if (d != e && !u->second.val && d->second.val)
		    {
		      error(d->second.loc,
			    "'properties: deterministic' contradicts...");
		      error(u->second.loc,
			    "... 'properties: !unambiguous' given here");
		    }
		}
	      auto sd = p.find("semi-deterministic");
	      if (sd != e)
		{
		  a->prop_semi_deterministic(sd->second.val);
		  auto d = p.find("deterministic");
		  if (d != e && !sd->second.val && d->second.val)
		    {
		      error(d->second.loc,
			    "'properties: deterministic' contradicts...");
		      error(sd->second.loc,
			    "... 'properties: !semi-deterministic' given here");
		    }
		}
	    }
	}
#line 1403 "parseaut.cc" // lalr1.cc:859
    break;

  case 17:
#line 646 "parseaut.yy" // lalr1.cc:859
    {
	   res.format_version = *(yystack_[0].value.str);
	   res.format_version_loc = yystack_[0].location;
	   delete (yystack_[0].value.str);
	 }
#line 1413 "parseaut.cc" // lalr1.cc:859
    break;

  case 18:
#line 652 "parseaut.yy" // lalr1.cc:859
    { res.h->loc = yystack_[0].location; }
#line 1419 "parseaut.cc" // lalr1.cc:859
    break;

  case 20:
#line 655 "parseaut.yy" // lalr1.cc:859
    {
       if (res.ignore_more_ap)
	 {
	   error(yystack_[1].location, "ignoring this redeclaration of APs...");
	   error(res.ap_loc, "... previously declared here.");
	 }
       else
	 {
	   res.ap_count = (yystack_[0].value.num);
	   res.ap.reserve((yystack_[0].value.num));
	 }
     }
#line 1436 "parseaut.cc" // lalr1.cc:859
    break;

  case 21:
#line 668 "parseaut.yy" // lalr1.cc:859
    {
       if (!res.ignore_more_ap)
	 {
	   res.ap_loc = yystack_[3].location + yystack_[2].location;
	   if ((int) res.ap.size() != res.ap_count)
	     {
	       std::ostringstream out;
	       out << "found " << res.ap.size()
		   << " atomic propositions instead of the "
		   << res.ap_count << " announced";
	       error(yylhs.location, out.str());
	     }
	   res.ignore_more_ap = true;
	 }
     }
#line 1456 "parseaut.cc" // lalr1.cc:859
    break;

  case 24:
#line 686 "parseaut.yy" // lalr1.cc:859
    {
	     if (res.states >= 0)
	       {
		 error(yylhs.location, "redefinition of the number of states...");
		 error(res.states_loc, "... previously defined here.");
	       }
	     else
	       {
		 res.states_loc = yylhs.location;
	       }
	     if (((int) (yystack_[0].value.num)) < 0)
	       {
		 error(yylhs.location, "too many states for this implementation");
		 YYABORT;
	       }
	     res.states = std::max(res.states, (int) (yystack_[0].value.num));
	   }
#line 1478 "parseaut.cc" // lalr1.cc:859
    break;

  case 25:
#line 704 "parseaut.yy" // lalr1.cc:859
    {
               res.start.emplace_back(yylhs.location, *(yystack_[0].value.states));
               delete (yystack_[0].value.states);
	     }
#line 1487 "parseaut.cc" // lalr1.cc:859
    break;

  case 26:
#line 709 "parseaut.yy" // lalr1.cc:859
    {
	       res.start.emplace_back(yylhs.location, std::vector<unsigned>{(yystack_[0].value.num)});
	     }
#line 1495 "parseaut.cc" // lalr1.cc:859
    break;

  case 28:
#line 714 "parseaut.yy" // lalr1.cc:859
    {
	       if (!res.alias.emplace(*(yystack_[1].value.str), bdd_from_int((yystack_[0].value.b))).second)
		 {
		   std::ostringstream o;
		   o << "ignoring redefinition of alias @" << *(yystack_[1].value.str);
		   error(yylhs.location, o.str());
		 }
	       delete (yystack_[1].value.str);
	       bdd_delref((yystack_[0].value.b));
	     }
#line 1510 "parseaut.cc" // lalr1.cc:859
    break;

  case 29:
#line 725 "parseaut.yy" // lalr1.cc:859
    {
		if (res.ignore_more_acc)
		  {
		    error(yystack_[1].location + yystack_[0].location, "ignoring this redefinition of the "
			  "acceptance condition...");
		    error(res.accset_loc, "... previously defined here.");
		  }
		else if ((yystack_[0].value.num) > 8 * sizeof(spot::acc_cond::mark_t::value_t))
		  {
		    error(yystack_[1].location + yystack_[0].location,
			  "this implementation cannot support such a large "
			  "number of acceptance sets");
		    YYABORT;
		  }
		else
		  {
		    res.aut_or_ks->acc().add_sets((yystack_[0].value.num));
		    res.accset = (yystack_[0].value.num);
		    res.accset_loc = yystack_[1].location + yystack_[0].location;
		  }
	     }
#line 1536 "parseaut.cc" // lalr1.cc:859
    break;

  case 30:
#line 747 "parseaut.yy" // lalr1.cc:859
    {
	       res.ignore_more_acc = true;
	       // Not setting the acceptance in case of error will
	       // force it to be true.
	       if (res.opts.want_kripke && (!(yystack_[0].value.code)->is_t() || (yystack_[2].value.num) > 0))
		 error(yystack_[2].location + yystack_[0].location,
		       "the acceptance for Kripke structure must be '0 t'");
	       else
		 res.aut_or_ks->set_acceptance((yystack_[2].value.num), *(yystack_[0].value.code));
	       delete (yystack_[0].value.code);
	     }
#line 1552 "parseaut.cc" // lalr1.cc:859
    break;

  case 31:
#line 759 "parseaut.yy" // lalr1.cc:859
    {
	       delete (yystack_[1].value.str);
	     }
#line 1560 "parseaut.cc" // lalr1.cc:859
    break;

  case 32:
#line 763 "parseaut.yy" // lalr1.cc:859
    {
	       delete (yystack_[1].value.str);
	       delete (yystack_[0].value.str);
	     }
#line 1569 "parseaut.cc" // lalr1.cc:859
    break;

  case 33:
#line 768 "parseaut.yy" // lalr1.cc:859
    {
	       res.aut_or_ks->set_named_prop("automaton-name", (yystack_[0].value.str));
	     }
#line 1577 "parseaut.cc" // lalr1.cc:859
    break;

  case 35:
#line 773 "parseaut.yy" // lalr1.cc:859
    { res.highlight_edges = new std::map<unsigned, unsigned>; }
#line 1583 "parseaut.cc" // lalr1.cc:859
    break;

  case 37:
#line 776 "parseaut.yy" // lalr1.cc:859
    { res.highlight_states = new std::map<unsigned, unsigned>; }
#line 1589 "parseaut.cc" // lalr1.cc:859
    break;

  case 39:
#line 779 "parseaut.yy" // lalr1.cc:859
    {
	       char c = (*(yystack_[1].value.str))[0];
	       if (c >= 'A' && c <= 'Z')
		 error(yylhs.location, "ignoring unsupported header \"" + *(yystack_[1].value.str) + ":\"\n\t"
		       "(but the capital indicates information that should not"
		       " be ignored)");
	       delete (yystack_[1].value.str);
	     }
#line 1602 "parseaut.cc" // lalr1.cc:859
    break;

  case 43:
#line 791 "parseaut.yy" // lalr1.cc:859
    {
	   if (!res.ignore_more_ap)
	     {
	       auto f = res.env->require(*(yystack_[0].value.str));
	       int b = 0;
	       if (f == nullptr)
		 {
		   std::ostringstream out;
		   out << "unknown atomic proposition \"" << *(yystack_[0].value.str) << "\"";
		   error(yystack_[0].location, out.str());
		   b = res.aut_or_ks->register_ap("$unknown$");
		 }
	       else
		 {
		   b = res.aut_or_ks->register_ap(f);
		   if (!res.ap_set.emplace(b).second)
		     {
		       std::ostringstream out;
		       out << "duplicate atomic proposition \"" << *(yystack_[0].value.str) << "\"";
		       error(yystack_[0].location, out.str());
		     }
		 }
	       res.ap.push_back(b);
	     }
	   delete (yystack_[0].value.str);
	 }
#line 1633 "parseaut.cc" // lalr1.cc:859
    break;

  case 47:
#line 821 "parseaut.yy" // lalr1.cc:859
    {
	      delete (yystack_[0].value.str);
	    }
#line 1641 "parseaut.cc" // lalr1.cc:859
    break;

  case 49:
#line 825 "parseaut.yy" // lalr1.cc:859
    {
                bool val = true;
                // no-univ-branch was replaced by !univ-branch in HOA 1.1
                if (*(yystack_[0].value.str) == "no-univ-branch")
                  {
                    *(yystack_[0].value.str) = "univ-branch";
                    val = false;
                  }
		auto pos = res.props.emplace(*(yystack_[0].value.str), result_::prop_info{yystack_[0].location, val});
		if (pos.first->second.val != val)
		  {
		    std::ostringstream out(std::ios_base::ate);
		    error(yystack_[0].location, std::string("'properties: ")
			  + (val ? "" : "!") + *(yystack_[0].value.str) + "' contradicts...");
		    error(pos.first->second.loc,
			  std::string("... 'properties: ")
                          + (val ? "!" : "") + *(yystack_[0].value.str)
			  + "' previously given here.");
		  }
		delete (yystack_[0].value.str);
	      }
#line 1667 "parseaut.cc" // lalr1.cc:859
    break;

  case 50:
#line 847 "parseaut.yy" // lalr1.cc:859
    {
		auto loc = yystack_[1].location + yystack_[0].location;
		auto pos =
		  res.props.emplace(*(yystack_[0].value.str), result_::prop_info{loc, false});
		if (pos.first->second.val)
		  {
		    std::ostringstream out(std::ios_base::ate);
		    error(loc, std::string("'properties: !")
			  + *(yystack_[0].value.str) + "' contradicts...");
		    error(pos.first->second.loc,
			  std::string("... 'properties: ") + *(yystack_[0].value.str)
			  + "' previously given here.");
		  }
		delete (yystack_[0].value.str);
	      }
#line 1687 "parseaut.cc" // lalr1.cc:859
    break;

  case 52:
#line 864 "parseaut.yy" // lalr1.cc:859
    {
		res.highlight_edges->emplace((yystack_[1].value.num), (yystack_[0].value.num));
	      }
#line 1695 "parseaut.cc" // lalr1.cc:859
    break;

  case 54:
#line 868 "parseaut.yy" // lalr1.cc:859
    {
		res.highlight_states->emplace((yystack_[1].value.num), (yystack_[0].value.num));
	      }
#line 1703 "parseaut.cc" // lalr1.cc:859
    break;

  case 58:
#line 875 "parseaut.yy" // lalr1.cc:859
    {
		 delete (yystack_[0].value.str);
	       }
#line 1711 "parseaut.cc" // lalr1.cc:859
    break;

  case 59:
#line 879 "parseaut.yy" // lalr1.cc:859
    {
		 delete (yystack_[0].value.str);
	       }
#line 1719 "parseaut.cc" // lalr1.cc:859
    break;

  case 60:
#line 884 "parseaut.yy" // lalr1.cc:859
    {
              (yylhs.value.states) = new std::vector<unsigned>{(yystack_[2].value.num), (yystack_[0].value.num)};
            }
#line 1727 "parseaut.cc" // lalr1.cc:859
    break;

  case 61:
#line 888 "parseaut.yy" // lalr1.cc:859
    {
              (yylhs.value.states) = (yystack_[2].value.states);
              (yylhs.value.states)->emplace_back((yystack_[0].value.num));
            }
#line 1736 "parseaut.cc" // lalr1.cc:859
    break;

  case 62:
#line 896 "parseaut.yy" // lalr1.cc:859
    {
              (yylhs.value.states) = new std::vector<unsigned>{(yystack_[2].value.num), (yystack_[0].value.num)};
            }
#line 1744 "parseaut.cc" // lalr1.cc:859
    break;

  case 63:
#line 900 "parseaut.yy" // lalr1.cc:859
    {
              (yylhs.value.states) = (yystack_[2].value.states);
              (yylhs.value.states)->emplace_back((yystack_[0].value.num));
            }
#line 1753 "parseaut.cc" // lalr1.cc:859
    break;

  case 64:
#line 906 "parseaut.yy" // lalr1.cc:859
    {
	      (yylhs.value.b) = bddtrue.id();
	    }
#line 1761 "parseaut.cc" // lalr1.cc:859
    break;

  case 65:
#line 910 "parseaut.yy" // lalr1.cc:859
    {
	      (yylhs.value.b) = bddfalse.id();
	    }
#line 1769 "parseaut.cc" // lalr1.cc:859
    break;

  case 66:
#line 914 "parseaut.yy" // lalr1.cc:859
    {
	      if ((yystack_[0].value.num) >= res.ap.size())
		{
		  error(yystack_[0].location, "AP number is larger than the number of APs...");
		  error(res.ap_loc, "... declared here");
		  (yylhs.value.b) = bddtrue.id();
		}
	      else
		{
		  (yylhs.value.b) = bdd_ithvar(res.ap[(yystack_[0].value.num)]).id();
		  bdd_addref((yylhs.value.b));
		}
	    }
#line 1787 "parseaut.cc" // lalr1.cc:859
    break;

  case 67:
#line 928 "parseaut.yy" // lalr1.cc:859
    {
	      auto i = res.alias.find(*(yystack_[0].value.str));
	      if (i == res.alias.end())
		{
		  error(yylhs.location, "unknown alias @" + *(yystack_[0].value.str));
		  (yylhs.value.b) = 1;
		}
	      else
		{
		  (yylhs.value.b) = i->second.id();
		  bdd_addref((yylhs.value.b));
		}
	      delete (yystack_[0].value.str);
	    }
#line 1806 "parseaut.cc" // lalr1.cc:859
    break;

  case 68:
#line 943 "parseaut.yy" // lalr1.cc:859
    {
              (yylhs.value.b) = bdd_not((yystack_[0].value.b));
              bdd_delref((yystack_[0].value.b));
              bdd_addref((yylhs.value.b));
            }
#line 1816 "parseaut.cc" // lalr1.cc:859
    break;

  case 69:
#line 949 "parseaut.yy" // lalr1.cc:859
    {
              (yylhs.value.b) = bdd_and((yystack_[2].value.b), (yystack_[0].value.b));
              bdd_delref((yystack_[2].value.b));
              bdd_delref((yystack_[0].value.b));
              bdd_addref((yylhs.value.b));
            }
#line 1827 "parseaut.cc" // lalr1.cc:859
    break;

  case 70:
#line 956 "parseaut.yy" // lalr1.cc:859
    {
              (yylhs.value.b) = bdd_or((yystack_[2].value.b), (yystack_[0].value.b));
              bdd_delref((yystack_[2].value.b));
              bdd_delref((yystack_[0].value.b));
              bdd_addref((yylhs.value.b));
            }
#line 1838 "parseaut.cc" // lalr1.cc:859
    break;

  case 71:
#line 963 "parseaut.yy" // lalr1.cc:859
    {
	    (yylhs.value.b) = (yystack_[1].value.b);
	  }
#line 1846 "parseaut.cc" // lalr1.cc:859
    break;

  case 72:
#line 969 "parseaut.yy" // lalr1.cc:859
    {
	      if ((int) (yystack_[0].value.num) >= res.accset)
		{
		  if (!res.ignore_acc)
		    {
		      error(yystack_[0].location, "number is larger than the count "
			    "of acceptance sets...");
		      error(res.accset_loc, "... declared here.");
		    }
		  (yylhs.value.num) = -1U;
		}
	      else
		{
		  (yylhs.value.num) = (yystack_[0].value.num);
		}
	    }
#line 1867 "parseaut.cc" // lalr1.cc:859
    break;

  case 73:
#line 987 "parseaut.yy" // lalr1.cc:859
    {
		   if ((yystack_[1].value.num) != -1U)
		     {
		       res.pos_acc_sets |= res.aut_or_ks->acc().mark((yystack_[1].value.num));
		       if (*(yystack_[3].value.str) == "Inf")
                         {
                           (yylhs.value.code) = new spot::acc_cond::acc_code
                             (res.aut_or_ks->acc().inf({(yystack_[1].value.num)}));
                         }
		       else if (*(yystack_[3].value.str) == "Fin")
                         {
                           (yylhs.value.code) = new spot::acc_cond::acc_code
                             (res.aut_or_ks->acc().fin({(yystack_[1].value.num)}));
                         }
                       else
                         {
                           error(yystack_[3].location, std::string("unknown acceptance '") + *(yystack_[3].value.str)
                                 + "', expected Fin or Inf");
                           (yylhs.value.code) = new spot::acc_cond::acc_code;
                         }
		     }
		   else
		     {
		       (yylhs.value.code) = new spot::acc_cond::acc_code;
		     }
		   delete (yystack_[3].value.str);
		 }
#line 1899 "parseaut.cc" // lalr1.cc:859
    break;

  case 74:
#line 1015 "parseaut.yy" // lalr1.cc:859
    {
		   if ((yystack_[1].value.num) != -1U)
		     {
		       res.neg_acc_sets |= res.aut_or_ks->acc().mark((yystack_[1].value.num));
		       if (*(yystack_[4].value.str) == "Inf")
			 (yylhs.value.code) = new spot::acc_cond::acc_code
			   (res.aut_or_ks->acc().inf_neg({(yystack_[1].value.num)}));
		       else
			 (yylhs.value.code) = new spot::acc_cond::acc_code
			   (res.aut_or_ks->acc().fin_neg({(yystack_[1].value.num)}));
		     }
		   else
		     {
		       (yylhs.value.code) = new spot::acc_cond::acc_code;
		     }
		   delete (yystack_[4].value.str);
		 }
#line 1921 "parseaut.cc" // lalr1.cc:859
    break;

  case 75:
#line 1033 "parseaut.yy" // lalr1.cc:859
    {
		   (yylhs.value.code) = (yystack_[1].value.code);
		 }
#line 1929 "parseaut.cc" // lalr1.cc:859
    break;

  case 76:
#line 1037 "parseaut.yy" // lalr1.cc:859
    {
		   *(yystack_[0].value.code) &= std::move(*(yystack_[2].value.code));
		   (yylhs.value.code) = (yystack_[0].value.code);
		   delete (yystack_[2].value.code);
		 }
#line 1939 "parseaut.cc" // lalr1.cc:859
    break;

  case 77:
#line 1043 "parseaut.yy" // lalr1.cc:859
    {
		   *(yystack_[0].value.code) |= std::move(*(yystack_[2].value.code));
		   (yylhs.value.code) = (yystack_[0].value.code);
		   delete (yystack_[2].value.code);
		 }
#line 1949 "parseaut.cc" // lalr1.cc:859
    break;

  case 78:
#line 1049 "parseaut.yy" // lalr1.cc:859
    {
		   (yylhs.value.code) = new spot::acc_cond::acc_code;
		 }
#line 1957 "parseaut.cc" // lalr1.cc:859
    break;

  case 79:
#line 1053 "parseaut.yy" // lalr1.cc:859
    {
	         {
		   (yylhs.value.code) = new spot::acc_cond::acc_code
		     (res.aut_or_ks->acc().fin({}));
		 }
	       }
#line 1968 "parseaut.cc" // lalr1.cc:859
    break;

  case 80:
#line 1062 "parseaut.yy" // lalr1.cc:859
    {
	for (auto& p: res.start)
          for (unsigned s: p.second)
            if (s >= res.info_states.size() || !res.info_states[s].declared)
              {
                error(p.first, "initial state " + std::to_string(s) +
                      " has no definition");
                // Pretend that the state is declared so we do not
                // mention it in the next loop.
                if (s < res.info_states.size())
                  res.info_states[s].declared = true;
                res.complete = spot::trival::maybe();
              }
	unsigned n = res.info_states.size();
	// States with number above res.states have already caused a
	// diagnostic, so let not add another one.
	if (res.states >= 0)
	  n = res.states;
	for (unsigned i = 0; i < n; ++i)
	  {
	    auto& p = res.info_states[i];
            if (!p.declared)
              {
                if (p.used)
                  error(p.used_loc,
                        "state " + std::to_string(i) + " has no definition");
                if (!p.used && res.complete)
                  if (auto p = res.prop_is_true("complete"))
                    {
                      error(res.states_loc,
                            "state " + std::to_string(i) +
                            " has no definition...");
                      error(p.loc, "... despite 'properties: complete'");
                    }
                res.complete = false;
              }
	  }
        if (res.complete)
          if (auto p = res.prop_is_false("complete"))
            {
              error(yystack_[0].location, "automaton is complete...");
              error(p.loc, "... despite 'properties: !complete'");
            }
        bool det_warned = false;
        if (res.universal && res.existential)
          if (auto p = res.prop_is_false("deterministic"))
            {
              error(yystack_[0].location, "automaton is deterministic...");
              error(p.loc, "... despite 'properties: !deterministic'");
              det_warned = true;
            }
        static bool tolerant = getenv("SPOT_HOA_TOLERANT");
        if (res.universal.is_true() && !det_warned && !tolerant)
          if (auto p = res.prop_is_true("exist-branch"))
            {
              error(yystack_[0].location, "automaton has no existential branching...");
              error(p.loc, "... despite 'properties: exist-branch'\n"
                    "note: If this is an issue you cannot fix, you may disable "
                    "this diagnostic\n      by defining the SPOT_HOA_TOLERANT "
                    "environment variable.");
              det_warned = true;
            }
        if (res.existential.is_true() && !det_warned && !tolerant)
          if (auto p = res.prop_is_true("univ-branch"))
            {
              error(yystack_[0].location, "automaton is has no universal branching...");
              error(p.loc, "... despite 'properties: univ-branch'\n"
                    "note: If this is an issue you cannot fix, you may disable "
                    "this diagnostic\n      by defining the SPOT_HOA_TOLERANT "
                    "environment variable.");
              det_warned = true;
            }
      }
#line 2046 "parseaut.cc" // lalr1.cc:859
    break;

  case 81:
#line 1136 "parseaut.yy" // lalr1.cc:859
    {
	     if (((int) (yystack_[0].value.num)) < 0)
	       {
		 error(yystack_[0].location, "state number is too large for this implementation");
		 YYABORT;
	       }
	     (yylhs.value.num) = (yystack_[0].value.num);
	   }
#line 2059 "parseaut.cc" // lalr1.cc:859
    break;

  case 82:
#line 1146 "parseaut.yy" // lalr1.cc:859
    {
		     if ((int) (yystack_[0].value.num) >= res.states)
		       {
			 if (res.states >= 0)
			   {
			     error(yystack_[0].location, "state number is larger than state "
				   "count...");
			     error(res.states_loc, "... declared here.");
			   }
			 if (res.opts.want_kripke)
			   {
			     int missing =
			       ((int) (yystack_[0].value.num)) - res.h->ks->num_states() + 1;
			     if (missing >= 0)
			       {
				 res.h->ks->new_states(missing, bddfalse);
				 res.info_states.resize
				   (res.info_states.size() + missing);
			       }
			   }
			 else
			   {
			     int missing =
			       ((int) (yystack_[0].value.num)) - res.h->aut->num_states() + 1;
			     if (missing >= 0)
			       {
				 res.h->aut->new_states(missing);
				 res.info_states.resize
				   (res.info_states.size() + missing);
			       }
			   }
		       }
		     // Remember the first place were a state is the
		     // destination of a transition.
		     if (!res.info_states[(yystack_[0].value.num)].used)
		       {
			 res.info_states[(yystack_[0].value.num)].used = true;
			 res.info_states[(yystack_[0].value.num)].used_loc = yystack_[0].location;
		       }
		     (yylhs.value.num) = (yystack_[0].value.num);
		   }
#line 2105 "parseaut.cc" // lalr1.cc:859
    break;

  case 84:
#line 1189 "parseaut.yy" // lalr1.cc:859
    {
	  if ((res.universal.is_true() || res.complete.is_true()))
	    {
	      bdd available = bddtrue;
	      bool det = true;
	      for (auto& t: res.h->aut->out(res.cur_state))
		{
		  if (det && !bdd_implies(t.cond, available))
		    det = false;
		  available -= t.cond;
		}
	      if (res.universal.is_true() && !det)
		{
		  res.universal = false;
		  if (auto p = res.prop_is_true("deterministic"))
		    {
		      error(yystack_[0].location, "automaton is not deterministic...");
		      error(p.loc,
			    "... despite 'properties: deterministic'");
		    }
		  else if (auto p = res.prop_is_false("exist-branch"))
		    {
		      error(yystack_[0].location, "automaton has existential branching...");
		      error(p.loc,
			    "... despite 'properties: !exist-branch'");
		    }
		}
	      if (res.complete.is_true() && available != bddfalse)
		{
		  res.complete = false;
		  if (auto p = res.prop_is_true("complete"))
		    {
		      error(yystack_[0].location, "automaton is not complete...");
		      error(p.loc, "... despite 'properties: complete'");
		    }
		}
	    }
	}
#line 2148 "parseaut.cc" // lalr1.cc:859
    break;

  case 86:
#line 1229 "parseaut.yy" // lalr1.cc:859
    {
	 if (!res.has_state_label) // Implicit labels
	   {
	     if (res.cur_guard != res.guards.end())
	       error(yylhs.location, "not enough transitions for this state");

	     if (res.label_style == State_Labels)
	       {
		 error(yystack_[0].location, "these transitions have implicit labels but the"
		       " automaton is...");
		 error(res.props["state-labels"].loc, "... declared with "
		       "'properties: state-labels'");
		 // Do not repeat this message.
		 res.label_style = Mixed_Labels;
	       }
	     res.cur_guard = res.guards.begin();
	   }
	 else if (res.opts.want_kripke)
	   {
	     res.h->ks->state_from_number(res.cur_state)->cond(res.state_label);
	   }

       }
#line 2176 "parseaut.cc" // lalr1.cc:859
    break;

  case 87:
#line 1253 "parseaut.yy" // lalr1.cc:859
    {
	 // Assume the worse.  This skips the tests about determinism
	 // we might perform on the state.
	 res.universal = spot::trival::maybe();
	 res.existential = spot::trival::maybe();
	 res.complete = spot::trival::maybe();
       }
#line 2188 "parseaut.cc" // lalr1.cc:859
    break;

  case 88:
#line 1263 "parseaut.yy" // lalr1.cc:859
    {
	    res.cur_state = (yystack_[2].value.num);
	    if (res.info_states[(yystack_[2].value.num)].declared)
	      {
		std::ostringstream o;
		o << "redeclaration of state " << (yystack_[2].value.num);
		error(yystack_[4].location + yystack_[2].location, o.str());
                // The additional transitions from extra states might
                // led us to believe that the automaton is complete
                // while it is not if we ignore them.
                if (res.complete.is_true())
                  res.complete = spot::trival::maybe();
	      }
	    res.info_states[(yystack_[2].value.num)].declared = true;
	    res.acc_state = (yystack_[0].value.mark);
	    if ((yystack_[1].value.str))
	      {
		if (!res.state_names)
		  res.state_names =
		    new std::vector<std::string>(res.states > 0 ?
						 res.states : 0);
		if (res.state_names->size() < (yystack_[2].value.num) + 1)
		  res.state_names->resize((yystack_[2].value.num) + 1);
		(*res.state_names)[(yystack_[2].value.num)] = std::move(*(yystack_[1].value.str));
		delete (yystack_[1].value.str);
	      }
	    if (res.opts.want_kripke && !res.has_state_label)
	      error(yylhs.location, "Kripke structures should have labeled states");
	  }
#line 2222 "parseaut.cc" // lalr1.cc:859
    break;

  case 89:
#line 1293 "parseaut.yy" // lalr1.cc:859
    {
             res.cur_label = bdd_from_int((yystack_[1].value.b));
             bdd_delref((yystack_[1].value.b));
	   }
#line 2231 "parseaut.cc" // lalr1.cc:859
    break;

  case 90:
#line 1298 "parseaut.yy" // lalr1.cc:859
    {
	     error(yylhs.location, "ignoring this invalid label");
	     res.cur_label = bddtrue;
	   }
#line 2240 "parseaut.cc" // lalr1.cc:859
    break;

  case 91:
#line 1302 "parseaut.yy" // lalr1.cc:859
    { res.has_state_label = false; }
#line 2246 "parseaut.cc" // lalr1.cc:859
    break;

  case 92:
#line 1304 "parseaut.yy" // lalr1.cc:859
    {
		 res.has_state_label = true;
		 res.state_label_loc = yystack_[0].location;
		 res.state_label = res.cur_label;
		 if (res.label_style == Trans_Labels
		     || res.label_style == Implicit_Labels)
		   {
		     error(yylhs.location,
			   "state label used although the automaton was...");
		     if (res.label_style == Trans_Labels)
		       error(res.props["trans-labels"].loc,
			     "... declared with 'properties: trans-labels'"
			     " here");
		     else
		       error(res.props["implicit-labels"].loc,
			     "... declared with 'properties: implicit-labels'"
			     " here");
		     // Do not show this error anymore.
		     res.label_style = Mixed_Labels;
		   }
	       }
#line 2272 "parseaut.cc" // lalr1.cc:859
    break;

  case 93:
#line 1326 "parseaut.yy" // lalr1.cc:859
    {
		   if (res.has_state_label)
		     {
		       error(yystack_[0].location, "cannot label this edge because...");
		       error(res.state_label_loc,
			     "... the state is already labeled.");
		       res.cur_label = res.state_label;
		     }
		   if (res.label_style == State_Labels
		       || res.label_style == Implicit_Labels)
		     {
		       error(yylhs.location, "transition label used although the "
			     "automaton was...");
		       if (res.label_style == State_Labels)
			 error(res.props["state-labels"].loc,
			       "... declared with 'properties: state-labels' "
			       "here");
		       else
			 error(res.props["implicit-labels"].loc,
			       "... declared with 'properties: implicit-labels'"
			       " here");
		       // Do not show this error anymore.
		       res.label_style = Mixed_Labels;
		     }
		 }
#line 2302 "parseaut.cc" // lalr1.cc:859
    break;

  case 94:
#line 1353 "parseaut.yy" // lalr1.cc:859
    {
	       (yylhs.value.mark) = (yystack_[1].value.mark);
	       if (res.ignore_acc && !res.ignore_acc_silent)
		 {
		   error(yylhs.location, "ignoring acceptance sets because of "
			 "missing acceptance condition");
		   // Emit this message only once.
		   res.ignore_acc_silent = true;
		 }
	     }
#line 2317 "parseaut.cc" // lalr1.cc:859
    break;

  case 95:
#line 1364 "parseaut.yy" // lalr1.cc:859
    {
	       error(yylhs.location, "ignoring this invalid acceptance set");
	     }
#line 2325 "parseaut.cc" // lalr1.cc:859
    break;

  case 96:
#line 1368 "parseaut.yy" // lalr1.cc:859
    {
	    (yylhs.value.mark) = spot::acc_cond::mark_t(0U);
	  }
#line 2333 "parseaut.cc" // lalr1.cc:859
    break;

  case 97:
#line 1372 "parseaut.yy" // lalr1.cc:859
    {
	    if (res.ignore_acc || (yystack_[0].value.num) == -1U)
	      (yylhs.value.mark) = spot::acc_cond::mark_t(0U);
	    else
	      (yylhs.value.mark) = (yystack_[1].value.mark) | res.aut_or_ks->acc().mark((yystack_[0].value.num));
	  }
#line 2344 "parseaut.cc" // lalr1.cc:859
    break;

  case 98:
#line 1380 "parseaut.yy" // lalr1.cc:859
    {
                 (yylhs.value.mark) = spot::acc_cond::mark_t(0U);
               }
#line 2352 "parseaut.cc" // lalr1.cc:859
    break;

  case 99:
#line 1384 "parseaut.yy" // lalr1.cc:859
    {
		 (yylhs.value.mark) = (yystack_[0].value.mark);
		 if (res.acc_style == Trans_Acc)
		   {
		     error(yylhs.location, "state-based acceptance used despite...");
		     error(res.props["trans-acc"].loc,
			   "... declaration of transition-based acceptance.");
		     res.acc_style = Mixed_Acc;
		   }
	       }
#line 2367 "parseaut.cc" // lalr1.cc:859
    break;

  case 100:
#line 1395 "parseaut.yy" // lalr1.cc:859
    {
                 (yylhs.value.mark) = spot::acc_cond::mark_t(0U);
               }
#line 2375 "parseaut.cc" // lalr1.cc:859
    break;

  case 101:
#line 1399 "parseaut.yy" // lalr1.cc:859
    {
		 (yylhs.value.mark) = (yystack_[0].value.mark);
		 res.trans_acc_seen = true;
		 if (res.acc_style == State_Acc)
		   {
		     error(yylhs.location, "trans-based acceptance used despite...");
		     error(res.props["state-acc"].loc,
			   "... declaration of state-based acceptance.");
		     res.acc_style = Mixed_Acc;
		   }
	       }
#line 2391 "parseaut.cc" // lalr1.cc:859
    break;

  case 107:
#line 1417 "parseaut.yy" // lalr1.cc:859
    {
			      bdd cond = bddtrue;
			      if (!res.has_state_label)
				error(yylhs.location, "missing label for this edge "
				      "(previous edge is labeled)");
			      else
				cond = res.state_label;
			      if (cond != bddfalse)
				{
				  if (res.opts.want_kripke)
				    res.h->ks->new_edge(res.cur_state, (yystack_[1].value.num));
				  else
				    res.h->aut->new_edge(res.cur_state, (yystack_[1].value.num),
							 cond,
							 (yystack_[0].value.mark) | res.acc_state);
				}
			    }
#line 2413 "parseaut.cc" // lalr1.cc:859
    break;

  case 108:
#line 1435 "parseaut.yy" // lalr1.cc:859
    {
		if (res.cur_label != bddfalse)
		  {
		    if (res.opts.want_kripke)
		      res.h->ks->new_edge(res.cur_state, (yystack_[1].value.num));
		    else
		      res.h->aut->new_edge(res.cur_state, (yystack_[1].value.num),
					   res.cur_label, (yystack_[0].value.mark) | res.acc_state);
		  }
	      }
#line 2428 "parseaut.cc" // lalr1.cc:859
    break;

  case 109:
#line 1446 "parseaut.yy" // lalr1.cc:859
    {
                if (res.cur_label != bddfalse)
                  {
                    assert(!res.opts.want_kripke);
                    res.h->aut->new_univ_edge(res.cur_state,
                                              (yystack_[1].value.states)->begin(), (yystack_[1].value.states)->end(),
                                              res.cur_label,
                                              (yystack_[0].value.mark) | res.acc_state);
                  }
                delete (yystack_[1].value.states);
	      }
#line 2444 "parseaut.cc" // lalr1.cc:859
    break;

  case 110:
#line 1459 "parseaut.yy" // lalr1.cc:859
    {
                (yylhs.value.states) = (yystack_[0].value.states);
                if (auto ub = res.prop_is_false("univ-branch"))
                  {
                    error(yystack_[0].location, "universal branch used despite"
                          " previous declaration...");
                    error(ub.loc, "... here");
                  }
                res.existential = false;
              }
#line 2459 "parseaut.cc" // lalr1.cc:859
    break;

  case 114:
#line 1477 "parseaut.yy" // lalr1.cc:859
    {
		  bdd cond;
		  if (res.has_state_label)
		    {
		      cond = res.state_label;
		    }
		  else
		    {
		      if (res.guards.empty())
			fill_guards(res);
		      if (res.cur_guard == res.guards.end())
			{
			  error(yylhs.location, "too many transitions for this state, "
				"ignoring this one");
			  cond = bddfalse;
			}
		      else
			{
			  cond = *res.cur_guard++;
			}
		    }
		  if (cond != bddfalse)
		    {
		      if (res.opts.want_kripke)
			res.h->ks->new_edge(res.cur_state, (yystack_[1].value.num));
		      else
			res.h->aut->new_edge(res.cur_state, (yystack_[1].value.num),
					     cond, (yystack_[0].value.mark) | res.acc_state);
		    }
		}
#line 2494 "parseaut.cc" // lalr1.cc:859
    break;

  case 115:
#line 1508 "parseaut.yy" // lalr1.cc:859
    {
		  bdd cond;
		  if (res.has_state_label)
		    {
		      cond = res.state_label;
		    }
		  else
		    {
		      if (res.guards.empty())
			fill_guards(res);
		      if (res.cur_guard == res.guards.end())
			{
			  error(yylhs.location, "too many transitions for this state, "
				"ignoring this one");
			  cond = bddfalse;
			}
		      else
			{
			  cond = *res.cur_guard++;
			}
		    }
		  if (cond != bddfalse)
		    {
		      assert(!res.opts.want_kripke);
                      res.h->aut->new_univ_edge(res.cur_state,
                                                (yystack_[1].value.states)->begin(), (yystack_[1].value.states)->end(),
                                                cond, (yystack_[0].value.mark) | res.acc_state);
		    }
                  delete (yystack_[1].value.states);
		}
#line 2529 "parseaut.cc" // lalr1.cc:859
    break;

  case 116:
#line 1539 "parseaut.yy" // lalr1.cc:859
    {
			    error(yystack_[1].location, "ignoring this label, because previous"
				  " edge has no label");
                          }
#line 2538 "parseaut.cc" // lalr1.cc:859
    break;

  case 118:
#line 1551 "parseaut.yy" // lalr1.cc:859
    {
	 error(yylhs.location, "failed to parse this as an ltl2dstar automaton");
       }
#line 2546 "parseaut.cc" // lalr1.cc:859
    break;

  case 119:
#line 1556 "parseaut.yy" // lalr1.cc:859
    {
         res.h->type = spot::parsed_aut_type::DRA;
         res.plus = 1;
         res.minus = 0;
	 if (res.opts.want_kripke)
	   {
	     error(yylhs.location,
		   "cannot read a Kripke structure out of a DSTAR automaton");
	     YYABORT;
	   }
       }
#line 2562 "parseaut.cc" // lalr1.cc:859
    break;

  case 120:
#line 1568 "parseaut.yy" // lalr1.cc:859
    {
	 res.h->type = spot::parsed_aut_type::DSA;
         res.plus = 0;
         res.minus = 1;
	 if (res.opts.want_kripke)
	   {
	     error(yylhs.location,
		   "cannot read a Kripke structure out of a DSTAR automaton");
	     YYABORT;
	   }
       }
#line 2578 "parseaut.cc" // lalr1.cc:859
    break;

  case 121:
#line 1581 "parseaut.yy" // lalr1.cc:859
    {
    if (res.states < 0)
      error(yystack_[0].location, "missing state count");
    if (!res.ignore_more_acc)
      error(yystack_[0].location, "missing acceptance-pair count");
    if (res.start.empty())
      error(yystack_[0].location, "missing start-state number");
    if (!res.ignore_more_ap)
      error(yystack_[0].location, "missing atomic propositions definition");

    if (res.states > 0)
      {
	res.h->aut->new_states(res.states);;
	res.info_states.resize(res.states);
      }
    res.acc_style = State_Acc;
    res.universal = true;
    res.existential = true;
    res.complete = true;
    fill_guards(res);
    res.cur_guard = res.guards.end();
  }
#line 2605 "parseaut.cc" // lalr1.cc:859
    break;

  case 124:
#line 1607 "parseaut.yy" // lalr1.cc:859
    {
    if (res.ignore_more_acc)
      {
	error(yystack_[2].location + yystack_[1].location, "ignoring this redefinition of the "
	      "acceptance pairs...");
	error(res.accset_loc, "... previously defined here.");
      }
    else{
      res.accset = (yystack_[0].value.num);
      res.h->aut->set_acceptance(2 * (yystack_[0].value.num),
				 res.h->type == spot::parsed_aut_type::DRA
				 ? spot::acc_cond::acc_code::rabin((yystack_[0].value.num))
				 : spot::acc_cond::acc_code::streett((yystack_[0].value.num)));
      res.accset_loc = yystack_[0].location;
      res.ignore_more_acc = true;
    }
  }
#line 2627 "parseaut.cc" // lalr1.cc:859
    break;

  case 125:
#line 1625 "parseaut.yy" // lalr1.cc:859
    {
    if (res.states < 0)
      {
	res.states = (yystack_[0].value.num);
      }
    else
      {
	error(yylhs.location, "redeclaration of state count");
	if ((unsigned) res.states < (yystack_[0].value.num))
	  res.states = (yystack_[0].value.num);
      }
  }
#line 2644 "parseaut.cc" // lalr1.cc:859
    break;

  case 126:
#line 1638 "parseaut.yy" // lalr1.cc:859
    {
    res.start.emplace_back(yystack_[0].location, std::vector<unsigned>{(yystack_[0].value.num)});
  }
#line 2652 "parseaut.cc" // lalr1.cc:859
    break;

  case 128:
#line 1644 "parseaut.yy" // lalr1.cc:859
    {
    if (res.cur_guard != res.guards.end())
      error(yystack_[2].location, "not enough transitions for previous state");
    if (res.states < 0 || (yystack_[1].value.num) >= (unsigned) res.states)
      {
	std::ostringstream o;
	if (res.states > 0)
	  {
	    o << "state numbers should be in the range [0.."
	      << res.states - 1 << "]";
	  }
	else
	  {
	    o << "no states have been declared";
	  }
	error(yystack_[1].location, o.str());
      }
    else
      {
	res.info_states[(yystack_[1].value.num)].declared = true;

	if ((yystack_[0].value.str))
	  {
	    if (!res.state_names)
	      res.state_names =
		new std::vector<std::string>(res.states > 0 ?
					     res.states : 0);
	    if (res.state_names->size() < (yystack_[1].value.num) + 1)
	      res.state_names->resize((yystack_[1].value.num) + 1);
	    (*res.state_names)[(yystack_[1].value.num)] = std::move(*(yystack_[0].value.str));
	    delete (yystack_[0].value.str);
	  }
      }

    res.cur_guard = res.guards.begin();
    res.dest_map.clear();
    res.cur_state = (yystack_[1].value.num);
  }
#line 2695 "parseaut.cc" // lalr1.cc:859
    break;

  case 129:
#line 1683 "parseaut.yy" // lalr1.cc:859
    { (yylhs.value.num) = res.plus; }
#line 2701 "parseaut.cc" // lalr1.cc:859
    break;

  case 130:
#line 1684 "parseaut.yy" // lalr1.cc:859
    { (yylhs.value.num) = res.minus; }
#line 2707 "parseaut.cc" // lalr1.cc:859
    break;

  case 131:
#line 1688 "parseaut.yy" // lalr1.cc:859
    {
    (yylhs.value.mark) = 0U;
  }
#line 2715 "parseaut.cc" // lalr1.cc:859
    break;

  case 132:
#line 1692 "parseaut.yy" // lalr1.cc:859
    {
    if (res.states < 0 || res.cur_state >= (unsigned) res.states)
      break;
    if (res.accset > 0 && (yystack_[0].value.num) < (unsigned) res.accset)
      {
	(yylhs.value.mark) = (yystack_[2].value.mark);
	(yylhs.value.mark).set((yystack_[0].value.num) * 2 + (yystack_[1].value.num));
      }
    else
      {
	std::ostringstream o;
	if (res.accset > 0)
	  {
	    o << "acceptance pairs should be in the range [0.."
	      << res.accset - 1 << "]";
	  }
	else
	  {
	    o << "no acceptance pairs have been declared";
	  }
	error(yystack_[0].location, o.str());
      }
  }
#line 2743 "parseaut.cc" // lalr1.cc:859
    break;

  case 133:
#line 1716 "parseaut.yy" // lalr1.cc:859
    { (yylhs.value.mark) = (yystack_[0].value.mark); }
#line 2749 "parseaut.cc" // lalr1.cc:859
    break;

  case 135:
#line 1720 "parseaut.yy" // lalr1.cc:859
    {
    std::pair<map_t::iterator, bool> i =
      res.dest_map.emplace((yystack_[0].value.num), *res.cur_guard);
    if (!i.second)
      i.first->second |= *res.cur_guard;
    ++res.cur_guard;
  }
#line 2761 "parseaut.cc" // lalr1.cc:859
    break;

  case 138:
#line 1731 "parseaut.yy" // lalr1.cc:859
    {
    for (auto i: res.dest_map)
      res.h->aut->new_edge(res.cur_state, i.first, i.second, (yystack_[1].value.mark));
  }
#line 2770 "parseaut.cc" // lalr1.cc:859
    break;

  case 139:
#line 1741 "parseaut.yy" // lalr1.cc:859
    {
	 if (res.opts.want_kripke)
	   {
	     error(yylhs.location, "cannot read a Kripke structure out of a never claim.");
	     YYABORT;
	   }
	 res.namer = res.h->aut->create_namer<std::string>();
	 res.h->aut->set_buchi();
	 res.acc_style = State_Acc;
	 res.pos_acc_sets = res.h->aut->acc().all_sets();
       }
#line 2786 "parseaut.cc" // lalr1.cc:859
    break;

  case 140:
#line 1753 "parseaut.yy" // lalr1.cc:859
    {
	 // Add an accept_all state if needed.
	 if (res.accept_all_needed && !res.accept_all_seen)
	   {
	     unsigned n = res.namer->new_state("accept_all");
	     res.h->aut->new_acc_edge(n, n, bddtrue);
	   }
	 // If we aliased existing state, we have some unreachable
	 // states to remove.
	 if (res.aliased_states)
	   res.h->aut->purge_unreachable_states();
	 res.info_states.resize(res.h->aut->num_states());
	 // Pretend that we have declared all states.
	 for (auto& p: res.info_states)
	   p.declared = true;
         res.h->aut->register_aps_from_dict();
       }
#line 2808 "parseaut.cc" // lalr1.cc:859
    break;

  case 145:
#line 1778 "parseaut.yy" // lalr1.cc:859
    {
      auto r = res.labels.insert(std::make_pair(*(yystack_[1].value.str), yystack_[1].location));
      if (!r.second)
	{
	  error(yystack_[1].location, std::string("redefinition of ") + *(yystack_[1].value.str) + "...");
	  error(r.first->second, std::string("... ")  + *(yystack_[1].value.str)
		+ " previously defined here");
	}
      (yylhs.value.str) = (yystack_[1].value.str);
    }
#line 2823 "parseaut.cc" // lalr1.cc:859
    break;

  case 146:
#line 1790 "parseaut.yy" // lalr1.cc:859
    {
      unsigned n = res.namer->new_state(*(yystack_[0].value.str));
      if (res.start.empty())
	{
	  // The first state is initial.
	  res.start.emplace_back(yylhs.location, std::vector<unsigned>{n});
	}
      (yylhs.value.str) = (yystack_[0].value.str);
    }
#line 2837 "parseaut.cc" // lalr1.cc:859
    break;

  case 147:
#line 1800 "parseaut.yy" // lalr1.cc:859
    {
      res.aliased_states |=
	res.namer->alias_state(res.namer->get_state(*(yystack_[1].value.str)), *(yystack_[0].value.str));
      // Keep any identifier that starts with accept.
      if (strncmp("accept", (yystack_[1].value.str)->c_str(), 6))
        {
          delete (yystack_[1].value.str);
          (yylhs.value.str) = (yystack_[0].value.str);
        }
      else
        {
	  delete (yystack_[0].value.str);
	  (yylhs.value.str) = (yystack_[1].value.str);
        }
    }
#line 2857 "parseaut.cc" // lalr1.cc:859
    break;

  case 148:
#line 1818 "parseaut.yy" // lalr1.cc:859
    {
      (yylhs.value.list) = (yystack_[1].value.list);
    }
#line 2865 "parseaut.cc" // lalr1.cc:859
    break;

  case 149:
#line 1822 "parseaut.yy" // lalr1.cc:859
    {
      (yylhs.value.list) = (yystack_[1].value.list);
    }
#line 2873 "parseaut.cc" // lalr1.cc:859
    break;

  case 150:
#line 1828 "parseaut.yy" // lalr1.cc:859
    {
      if (*(yystack_[1].value.str) == "accept_all")
	res.accept_all_seen = true;

      auto acc = !strncmp("accept", (yystack_[1].value.str)->c_str(), 6) ?
	res.h->aut->acc().all_sets() : spot::acc_cond::mark_t(0U);
      res.namer->new_edge(*(yystack_[1].value.str), *(yystack_[1].value.str), bddtrue, acc);
      delete (yystack_[1].value.str);
    }
#line 2887 "parseaut.cc" // lalr1.cc:859
    break;

  case 151:
#line 1837 "parseaut.yy" // lalr1.cc:859
    { delete (yystack_[0].value.str); }
#line 2893 "parseaut.cc" // lalr1.cc:859
    break;

  case 152:
#line 1838 "parseaut.yy" // lalr1.cc:859
    { delete (yystack_[1].value.str); }
#line 2899 "parseaut.cc" // lalr1.cc:859
    break;

  case 153:
#line 1840 "parseaut.yy" // lalr1.cc:859
    {
      auto acc = !strncmp("accept", (yystack_[1].value.str)->c_str(), 6) ?
	res.h->aut->acc().all_sets() : spot::acc_cond::mark_t(0U);
      for (auto& p: *(yystack_[0].value.list))
	{
	  bdd c = bdd_from_int(p.first);
	  bdd_delref(p.first);
	  res.namer->new_edge(*(yystack_[1].value.str), *p.second, c, acc);
	  delete p.second;
	}
      delete (yystack_[1].value.str);
      delete (yystack_[0].value.list);
    }
#line 2917 "parseaut.cc" // lalr1.cc:859
    break;

  case 154:
#line 1855 "parseaut.yy" // lalr1.cc:859
    { (yylhs.value.list) = new std::list<pair>; }
#line 2923 "parseaut.cc" // lalr1.cc:859
    break;

  case 155:
#line 1857 "parseaut.yy" // lalr1.cc:859
    {
      if ((yystack_[0].value.p))
	{
	  (yystack_[1].value.list)->push_back(*(yystack_[0].value.p));
	  delete (yystack_[0].value.p);
	}
      (yylhs.value.list) = (yystack_[1].value.list);
    }
#line 2936 "parseaut.cc" // lalr1.cc:859
    break;

  case 158:
#line 1869 "parseaut.yy" // lalr1.cc:859
    {
       auto i = res.fcache.find(*(yystack_[0].value.str));
       if (i == res.fcache.end())
	 {
	   auto pf = spot::parse_infix_boolean(*(yystack_[0].value.str), *res.env, debug_level(),
					       true);
	   for (auto& j: pf.errors)
	     {
	       // Adjust the diagnostic to the current position.
	       spot::location here = yystack_[0].location;
	       here.end.line = here.begin.line + j.first.end.line - 1;
	       here.end.column = here.begin.column + j.first.end.column - 1;
	       here.begin.line += j.first.begin.line - 1;
	       here.begin.column += j.first.begin.column - 1;
	       res.h->errors.emplace_back(here, j.second);
	     }
           bdd cond = bddfalse;
	   if (pf.f)
	     cond = spot::formula_to_bdd(pf.f,
					 res.h->aut->get_dict(), res.h->aut);
	   (yylhs.value.b) = (res.fcache[*(yystack_[0].value.str)] = cond).id();
	 }
       else
	 {
	   (yylhs.value.b) = i->second.id();
	 }
       bdd_addref((yylhs.value.b));
       delete (yystack_[0].value.str);
     }
#line 2970 "parseaut.cc" // lalr1.cc:859
    break;

  case 159:
#line 1899 "parseaut.yy" // lalr1.cc:859
    {
       (yylhs.value.b) = 0;
     }
#line 2978 "parseaut.cc" // lalr1.cc:859
    break;

  case 160:
#line 1905 "parseaut.yy" // lalr1.cc:859
    {
      (yylhs.value.str) = nullptr;
    }
#line 2986 "parseaut.cc" // lalr1.cc:859
    break;

  case 161:
#line 1909 "parseaut.yy" // lalr1.cc:859
    {
      (yylhs.value.str) = (yystack_[0].value.str);
    }
#line 2994 "parseaut.cc" // lalr1.cc:859
    break;

  case 162:
#line 1913 "parseaut.yy" // lalr1.cc:859
    {
      delete (yystack_[0].value.str);
      (yylhs.value.str) = new std::string("accept_all");
      res.accept_all_needed = true;
    }
#line 3004 "parseaut.cc" // lalr1.cc:859
    break;

  case 163:
#line 1920 "parseaut.yy" // lalr1.cc:859
    {
      // If there is no destination, do ignore the transition.
      // This happens for instance with
      //   if
      //   :: false
      //   fi
      if (!(yystack_[0].value.str))
	{
	  (yylhs.value.p) = nullptr;
	}
      else
	{
	  (yylhs.value.p) = new pair((yystack_[1].value.b), (yystack_[0].value.str));
	  res.namer->new_state(*(yystack_[0].value.str));
	}
    }
#line 3025 "parseaut.cc" // lalr1.cc:859
    break;

  case 164:
#line 1939 "parseaut.yy" // lalr1.cc:859
    {
      (yylhs.value.p) = (yystack_[1].value.p);
    }
#line 3033 "parseaut.cc" // lalr1.cc:859
    break;

  case 165:
#line 1943 "parseaut.yy" // lalr1.cc:859
    {
      (yylhs.value.p) = (yystack_[0].value.p);
    }
#line 3041 "parseaut.cc" // lalr1.cc:859
    break;

  case 166:
#line 1952 "parseaut.yy" // lalr1.cc:859
    {
	auto& acc = res.h->aut->acc();
	unsigned num = acc.num_sets();
	res.h->aut->set_generalized_buchi(num);
	res.pos_acc_sets = acc.all_sets();
	assert(!res.states_map.empty());
	auto n = res.states_map.size();
	if (n != (unsigned) res.states)
	  {
	    std::ostringstream err;
	    err << res.states << " states have been declared, but "
		<< n << " different state numbers have been used";
	    error(yylhs.location, err.str());
	  }
	if (res.states_map.rbegin()->first > (unsigned) res.states)
	  {
	    // We have seen numbers larger that the total number of
	    // states in the automaton.  Usually this happens when the
	    // states are numbered from 1 instead of 0, but the LBTT
	    // documentation actually allow any number to be used.
	    // What we have done is to map all input state numbers 0
	    // <= .. < n to the digraph states with the same number,
	    // and any time we saw a number larger than n, we mapped
	    // it to a new state.  The correspondence is given by
	    // res.states_map.  Now we just need to remove the useless
	    // states we allocated.
	    std::vector<unsigned> rename(res.h->aut->num_states(), -1U);
	    unsigned s = 0;
	    for (auto& i: res.states_map)
	      rename[i.second] = s++;
	    assert(s == (unsigned) res.states);
	    for (auto& i: res.start)
	      i.second.front() = rename[i.second.front()];
	    res.h->aut->get_graph().defrag_states(std::move(rename), s);
	  }
	 res.info_states.resize(res.h->aut->num_states());
	 for (auto& s: res.info_states)
	   s.declared = true;
         res.h->aut->register_aps_from_dict();
      }
#line 3086 "parseaut.cc" // lalr1.cc:859
    break;

  case 167:
#line 1993 "parseaut.yy" // lalr1.cc:859
    {
        res.h->aut->set_generalized_buchi((yystack_[0].value.num));
	res.pos_acc_sets = res.h->aut->acc().all_sets();
      }
#line 3095 "parseaut.cc" // lalr1.cc:859
    break;

  case 168:
#line 1999 "parseaut.yy" // lalr1.cc:859
    {
		    if (res.opts.want_kripke)
		      {
			error(yylhs.location,
			      "cannot read a Kripke structure out of "
			      "an LBTT automaton");
			YYABORT;
		      }
		    res.states = (yystack_[0].value.num);
		    res.states_loc = yystack_[0].location;
		    res.h->aut->new_states((yystack_[0].value.num));
		  }
#line 3112 "parseaut.cc" // lalr1.cc:859
    break;

  case 169:
#line 2012 "parseaut.yy" // lalr1.cc:859
    {
	     res.acc_mapper = new spot::acc_mapper_int(res.h->aut, (yystack_[0].value.num));
	     res.acc_style = State_Acc;
	   }
#line 3121 "parseaut.cc" // lalr1.cc:859
    break;

  case 170:
#line 2017 "parseaut.yy" // lalr1.cc:859
    {
	     res.acc_mapper = new spot::acc_mapper_int(res.h->aut, (yystack_[0].value.num));
	     res.trans_acc_seen = true;
	   }
#line 3130 "parseaut.cc" // lalr1.cc:859
    break;

  case 174:
#line 2026 "parseaut.yy" // lalr1.cc:859
    {
	    if ((yystack_[2].value.num) >= (unsigned) res.states)
	      {
		auto p = res.states_map.emplace((yystack_[2].value.num), 0);
		if (p.second)
		  p.first->second = res.h->aut->new_state();
		res.cur_state = p.first->second;
	      }
	    else
	      {
		res.states_map.emplace((yystack_[2].value.num), (yystack_[2].value.num));
		res.cur_state = (yystack_[2].value.num);
	      }
	    if ((yystack_[1].value.num))
	      res.start.emplace_back(yystack_[2].location + yystack_[1].location,
                                     std::vector<unsigned>{res.cur_state});
	    res.acc_state = (yystack_[0].value.mark);
	  }
#line 3153 "parseaut.cc" // lalr1.cc:859
    break;

  case 175:
#line 2044 "parseaut.yy" // lalr1.cc:859
    { (yylhs.value.mark) = 0U; }
#line 3159 "parseaut.cc" // lalr1.cc:859
    break;

  case 176:
#line 2046 "parseaut.yy" // lalr1.cc:859
    {
	  (yylhs.value.mark)  = (yystack_[1].value.mark);
	  auto p = res.acc_mapper->lookup((yystack_[0].value.num));
	  if (p.first)
	    (yylhs.value.mark) |= p.second;
	  else
	    error(yystack_[0].location, "more acceptance sets used than declared");
	}
#line 3172 "parseaut.cc" // lalr1.cc:859
    break;

  case 177:
#line 2055 "parseaut.yy" // lalr1.cc:859
    {
	    auto pf = spot::parse_prefix_ltl(*(yystack_[0].value.str), *res.env);
	    if (!pf.f || !pf.errors.empty())
	      {
		std::string s = "failed to parse guard: ";
		s += *(yystack_[0].value.str);
		error(yylhs.location, s);
	      }
	    if (!pf.errors.empty())
	      for (auto& j: pf.errors)
		{
		  // Adjust the diagnostic to the current position.
		  spot::location here = yystack_[0].location;
		  here.end.line = here.begin.line + j.first.end.line - 1;
		  here.end.column = here.begin.column + j.first.end.column - 1;
		  here.begin.line += j.first.begin.line - 1;
		  here.begin.column += j.first.begin.column - 1;
		  res.h->errors.emplace_back(here, j.second);
		}
	    if (!pf.f)
	      {
		res.cur_label = bddtrue;
	      }
	    else
	      {
		if (!pf.f.is_boolean())
		  {
		    error(yylhs.location,
			  "non-Boolean transition label (replaced by true)");
		    res.cur_label = bddtrue;
		  }
		else
		  {
		    res.cur_label =
		      formula_to_bdd(pf.f, res.h->aut->get_dict(), res.h->aut);
		  }
	      }
	    delete (yystack_[0].value.str);
	  }
#line 3216 "parseaut.cc" // lalr1.cc:859
    break;

  case 179:
#line 2096 "parseaut.yy" // lalr1.cc:859
    {
		  unsigned dst = (yystack_[2].value.num);
		  if (dst >= (unsigned) res.states)
		    {
		      auto p = res.states_map.emplace(dst, 0);
		      if (p.second)
			p.first->second = res.h->aut->new_state();
		      dst = p.first->second;
		    }
		  else
		    {
		      res.states_map.emplace(dst, dst);
		    }
		  res.h->aut->new_edge(res.cur_state, dst,
				       res.cur_label,
				       res.acc_state | (yystack_[1].value.mark));
		}
#line 3238 "parseaut.cc" // lalr1.cc:859
    break;


#line 3242 "parseaut.cc" // lalr1.cc:859
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


  const short int parser::yypact_ninf_ = -187;

  const signed char parser::yytable_ninf_ = -122;

  const short int
  parser::yypact_[] =
  {
      10,  -187,    44,    14,  -187,  -187,  -187,  -187,    18,  -187,
    -187,    13,  -187,  -187,   107,  -187,  -187,    73,  -187,  -187,
    -187,    22,    76,    39,  -187,  -187,   137,    57,    93,  -187,
    -187,  -187,    90,    86,  -187,  -187,  -187,   121,   145,   114,
    -187,   129,   138,   139,   142,   141,   146,   144,   147,  -187,
    -187,  -187,  -187,  -187,  -187,  -187,  -187,  -187,   148,  -187,
     102,   -32,  -187,    99,  -187,  -187,  -187,   110,  -187,    47,
    -187,  -187,   140,   143,  -187,    34,  -187,  -187,   151,  -187,
      98,  -187,  -187,    17,   149,    80,  -187,   117,  -187,  -187,
     121,  -187,  -187,  -187,  -187,  -187,  -187,     5,  -187,   138,
     150,  -187,    51,  -187,   138,  -187,    47,  -187,   116,    47,
    -187,   138,   138,  -187,  -187,  -187,    34,  -187,  -187,    34,
      66,    -4,    58,  -187,  -187,  -187,   155,   154,   157,  -187,
    -187,  -187,  -187,  -187,  -187,  -187,  -187,   158,   160,   161,
    -187,   127,  -187,  -187,    54,    61,   126,    62,   151,   138,
     138,     2,  -187,  -187,    51,   116,   116,  -187,  -187,  -187,
     138,  -187,  -187,  -187,  -187,   164,  -187,    71,    34,    34,
     131,  -187,  -187,    -4,    83,  -187,  -187,  -187,  -187,   165,
     166,    15,  -187,  -187,  -187,  -187,    -9,  -187,   125,  -187,
    -187,  -187,  -187,   116,  -187,  -187,   130,   -14,  -187,  -187,
    -187,  -187,  -187,  -187,  -187,   162,  -187,    -3,    75,    -4,
      -4,  -187,  -187,  -187,   169,  -187,   167,  -187,  -187,    82,
    -187,  -187,  -187,  -187,  -187,  -187,   170,   152,  -187,   163,
    -187,   151,  -187,  -187,  -187,  -187,   153,  -187,  -187,   159,
    -187,   168,  -187,  -187,    96,   174,     4,   112,  -187,  -187,
    -187,  -187,   175,  -187,   156,   181,   171,  -187,  -187,  -187,
    -187
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,     3,     0,     0,   119,   120,   139,   168,     0,     2,
       6,     0,    22,     9,     0,     7,     8,     0,   172,     4,
       5,     0,     0,     0,     1,    83,     0,     0,     0,   170,
     169,   167,     0,   171,    11,    17,    19,   141,     0,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      35,    37,    55,    27,    23,   118,   122,   166,     0,   178,
       0,     0,   146,   151,   142,    10,    87,    91,    84,   102,
      24,    81,    25,    26,    20,     0,    29,    44,    12,    33,
      34,    51,    53,    39,     0,     0,   175,   173,   145,   140,
     144,   150,   154,   154,   152,   147,   153,     0,    92,     0,
     110,    82,   100,    93,     0,    85,   103,   104,   100,    86,
     111,     0,     0,    41,    67,    66,     0,    64,    65,     0,
      28,     0,    31,    13,    32,    49,     0,    36,    38,    59,
      58,    57,    14,    15,    56,   136,   123,     0,     0,     0,
     127,   174,   175,   143,     0,     0,     0,     0,    12,     0,
       0,     0,   101,   114,   100,   100,   100,   106,   105,   115,
       0,   112,   113,    63,    62,    21,    68,     0,     0,     0,
       0,    78,    79,     0,    30,    47,    46,    45,    50,     0,
       0,     0,   125,   126,   124,   176,     0,   148,     0,   155,
     149,    90,    89,    98,    61,    60,     0,     0,   108,   109,
     107,   116,    43,    42,    71,    70,    69,     0,     0,     0,
       0,    52,    54,   137,     0,   117,     0,   177,   179,     0,
      99,    88,    95,    72,    94,    97,     0,     0,    75,    77,
      76,    12,   131,   134,   157,   159,     0,   156,   158,   160,
     165,     0,    73,   128,   133,   138,     0,     0,   163,    74,
     129,   130,     0,   135,     0,     0,     0,   132,   164,   161,
     162
  };

  const short int
  parser::yypgoto_[] =
  {
    -187,  -187,   198,  -187,  -148,    79,  -187,  -187,  -187,  -187,
     118,  -187,  -187,  -187,  -187,  -187,  -187,  -187,  -187,  -187,
    -187,  -187,  -187,  -187,  -187,  -187,   -96,  -186,  -131,  -187,
     -37,   -97,  -187,  -187,  -187,   135,  -187,    95,    12,  -187,
    -187,   -91,  -187,  -187,  -187,   100,   103,  -187,  -105,  -187,
    -187,  -187,  -187,  -187,  -187,  -187,  -187,  -187,  -187,  -187,
    -187,  -187,  -187,   172,  -187,  -187,   119,   120,  -187,  -187,
    -187,   -36,  -187,  -187,  -187,  -187,  -187,  -187,  -187,    69,
    -187,  -187
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,     8,     9,    10,   124,   134,    11,    36,    12,    22,
      53,   113,    26,    54,   121,    81,    82,   165,   203,   122,
      80,   127,   128,    83,   100,    72,   120,   225,   174,    38,
     101,   102,    39,    68,    69,   103,    99,   104,   152,   197,
     221,   153,   105,   106,   157,   107,   108,   109,   110,   162,
      13,    14,    84,    85,   216,   252,   244,   233,   245,   181,
      15,    23,    61,    62,    63,    96,    64,   144,   238,   239,
     248,   240,   189,    16,    17,    18,    32,    33,    59,   141,
     218,    87
  };

  const short int
  parser::yytable_[] =
  {
     193,   147,   148,   196,   161,    73,   146,   154,   223,   156,
       1,     2,   217,     3,   170,    21,   213,   159,    24,   223,
     166,   227,   234,   167,   -96,   114,    25,   115,    89,   226,
     214,    90,   -18,     4,     5,   129,    34,   116,   130,   131,
     241,   185,   208,     6,    19,   235,   224,     3,   237,   171,
     172,   173,   194,   195,   114,   201,   115,     7,   117,   118,
     119,   215,   -96,   198,   199,   200,   116,     4,     5,    71,
     132,   133,   205,   206,   163,   164,   175,     6,   229,   230,
     176,   136,   150,   243,   137,   138,    43,   117,   118,   119,
     187,     7,   168,   169,    35,    29,   168,   169,    37,   190,
     234,   168,   169,    55,    97,   209,   210,   139,    27,  -121,
     151,   132,   133,   209,   210,    66,   125,    60,   188,    56,
     192,    30,    31,   235,   236,   188,   237,   204,   -80,    67,
     126,   228,    28,    91,    92,    57,    93,    58,    40,    60,
      94,    41,    42,    43,    44,    45,    46,    47,    48,    49,
     -16,    70,   255,    50,    51,   256,    52,   250,   251,    65,
      71,    74,    75,    76,    77,    78,    88,    97,    79,   142,
      86,   111,   123,   178,   112,   151,   179,   185,   135,   180,
     182,   149,   183,   184,   191,   202,   207,   211,   212,   219,
     222,   231,   223,   169,   210,   232,   253,   257,   247,   259,
      20,   177,    98,   140,   160,   220,   158,   155,   242,   143,
     254,   186,   246,   145,     0,   260,   258,     0,     0,     0,
       0,     0,     0,     0,   249,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95
  };

  const short int
  parser::yycheck_[] =
  {
     148,    97,    99,     1,   109,    42,     1,   104,    22,   106,
       0,     1,    21,     3,    18,     1,     1,   108,     0,    22,
     116,   207,    18,   119,    22,    20,    13,    22,    60,    32,
      15,    63,    18,    23,    24,    18,    14,    32,    21,    22,
     226,    50,   173,    33,     0,    41,    60,     3,    44,    53,
      54,    55,   149,   150,    20,   160,    22,    47,    53,    54,
      55,    46,    60,   154,   155,   156,    32,    23,    24,    22,
      53,    54,   168,   169,   111,   112,    18,    33,   209,   210,
      22,     1,    31,   231,     4,     5,     6,    53,    54,    55,
      36,    47,    30,    31,    18,    22,    30,    31,    59,    38,
      18,    30,    31,    46,    57,    30,    31,    27,     1,    29,
      59,    53,    54,    30,    31,     1,    18,    18,    64,    26,
      58,    48,    49,    41,    42,    64,    44,    56,    14,    15,
      32,    56,    25,    34,    35,    45,    37,    51,     1,    18,
      41,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    22,    40,    16,    17,    43,    19,    61,    62,    14,
      22,    22,    20,    22,    18,    21,    64,    57,    21,    52,
      22,    31,    21,    18,    31,    59,    22,    50,    29,    22,
      22,    31,    22,    22,    58,    21,    55,    22,    22,    64,
      60,    22,    22,    31,    31,    28,    22,    22,    39,    18,
       2,   122,    67,    85,   109,   193,   106,   104,    56,    90,
     246,   142,    59,    93,    -1,    44,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     0,     1,     3,    23,    24,    33,    47,    66,    67,
      68,    71,    73,   115,   116,   125,   138,   139,   140,     0,
      67,     1,    74,   126,     0,    13,    77,     1,    25,    22,
      48,    49,   141,   142,    14,    18,    72,    59,    94,    97,
       1,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      16,    17,    19,    75,    78,    46,    26,    45,    51,   143,
      18,   127,   128,   129,   131,    14,     1,    15,    98,    99,
      22,    22,    90,    95,    22,    20,    22,    18,    21,    21,
      85,    80,    81,    88,   117,   118,    22,   146,    64,    60,
      63,    34,    35,    37,    41,   128,   130,    57,   100,   101,
      89,    95,    96,   100,   102,   107,   108,   110,   111,   112,
     113,    31,    31,    76,    20,    22,    32,    53,    54,    55,
      91,    79,    84,    21,    69,    18,    32,    86,    87,    18,
      21,    22,    53,    54,    70,    29,     1,     4,     5,    27,
      75,   144,    52,   131,   132,   132,     1,    91,    96,    31,
      31,    59,   103,   106,    96,   111,    96,   109,   110,   106,
     102,   113,   114,    95,    95,    82,    91,    91,    30,    31,
      18,    53,    54,    55,    93,    18,    22,    70,    18,    22,
      22,   124,    22,    22,    22,    50,   144,    36,    64,   137,
      38,    58,    58,    69,    96,    96,     1,   104,   106,   106,
     106,   113,    21,    83,    56,    91,    91,    55,    93,    30,
      31,    22,    22,     1,    15,    46,   119,    21,   145,    64,
     103,   105,    60,    22,    60,    92,    32,    92,    56,    93,
      93,    22,    28,   122,    18,    41,    42,    44,   133,   134,
     136,    92,    56,    69,   121,   123,    59,    39,   135,    56,
      61,    62,   120,    22,   136,    40,    43,    22,    60,    18,
      44
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    65,    66,    66,    66,    66,    67,    67,    67,    67,
      68,    68,    69,    69,    70,    70,    71,    72,    74,    73,
      76,    75,    77,    77,    78,    78,    78,    78,    78,    79,
      78,    78,    78,    78,    78,    80,    78,    81,    78,    78,
      78,    82,    82,    83,    84,    84,    84,    84,    85,    85,
      85,    86,    86,    87,    87,    88,    88,    88,    88,    88,
      89,    89,    90,    90,    91,    91,    91,    91,    91,    91,
      91,    91,    92,    93,    93,    93,    93,    93,    93,    93,
      94,    95,    96,    97,    97,    98,    98,    98,    99,   100,
     100,   101,   101,   102,   103,   103,   104,   104,   105,   105,
     106,   106,   107,   107,   108,   108,   108,   109,   110,   110,
     111,   112,   112,   112,   113,   113,   114,   115,   115,   116,
     116,   117,   118,   118,   118,   118,   118,   118,   119,   120,
     120,   121,   121,   122,   123,   123,   124,   124,   124,   126,
     125,   127,   127,   127,   127,   128,   129,   129,   130,   130,
     131,   131,   131,   131,   132,   132,   133,   133,   134,   134,
     135,   135,   135,   136,   137,   137,   138,   138,   139,   140,
     140,   141,   142,   142,   143,   144,   144,   145,   146,   146
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     1,     1,     2,     2,     1,     1,     1,     1,
       4,     3,     0,     1,     1,     1,     2,     1,     0,     3,
       0,     4,     0,     2,     2,     2,     2,     1,     3,     0,
       4,     3,     3,     2,     2,     0,     3,     0,     3,     2,
       1,     0,     2,     1,     0,     2,     2,     2,     0,     2,
       3,     0,     3,     0,     3,     0,     2,     2,     2,     2,
       3,     3,     3,     3,     1,     1,     1,     1,     2,     3,
       3,     3,     1,     4,     5,     3,     3,     3,     1,     1,
       1,     1,     1,     0,     2,     2,     2,     1,     5,     3,
       3,     0,     1,     1,     3,     3,     0,     2,     0,     1,
       0,     1,     0,     1,     1,     2,     2,     2,     3,     3,
       1,     1,     2,     2,     2,     2,     2,     7,     3,     1,
       1,     1,     0,     2,     3,     3,     3,     2,     3,     1,
       1,     0,     3,     2,     0,     2,     0,     2,     4,     0,
       5,     0,     1,     3,     2,     2,     1,     2,     3,     3,
       2,     1,     2,     2,     0,     2,     1,     1,     1,     1,
       0,     3,     3,     2,     6,     3,     3,     2,     1,     2,
       2,     1,     0,     3,     3,     0,     2,     1,     0,     4
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\"HOA:\"", "\"States:\"",
  "\"Start:\"", "\"AP:\"", "\"Alias:\"", "\"Acceptance:\"",
  "\"acc-name:\"", "\"tool:\"", "\"name:\"", "\"properties:\"",
  "\"--BODY--\"", "\"--END--\"", "\"State:\"", "\"spot.highlight.edges:\"",
  "\"spot.highlight.states:\"", "\"identifier\"", "\"header name\"",
  "\"alias name\"", "\"string\"", "\"integer\"", "\"DRA\"", "\"DSA\"",
  "\"v2\"", "\"explicit\"", "\"Acceptance-Pairs:\"", "\"Acc-Sig:\"",
  "\"---\"", "'|'", "'&'", "'!'", "\"never\"", "\"skip\"", "\"if\"",
  "\"fi\"", "\"do\"", "\"od\"", "\"->\"", "\"goto\"", "\"false\"",
  "\"atomic\"", "\"assert\"", "\"boolean formula\"", "\"-1\"",
  "\"end of DSTAR automaton\"", "\"LBTT header\"", "\"state acceptance\"",
  "\"acceptance sets for empty automaton\"", "\"acceptance set\"",
  "\"state number\"", "\"destination number\"", "'t'", "'f'", "'('", "')'",
  "'['", "']'", "'{'", "'}'", "'+'", "'-'", "';'", "':'", "$accept", "aut",
  "aut-1", "hoa", "string_opt", "BOOLEAN", "header", "version",
  "format-version", "$@1", "aps", "$@2", "header-items", "header-item",
  "$@3", "$@4", "$@5", "ap-names", "ap-name", "acc-spec", "properties",
  "highlight-edges", "highlight-states", "header-spec", "state-conj-2",
  "init-state-conj-2", "label-expr", "acc-set", "acceptance-cond", "body",
  "state-num", "checked-state-num", "states", "state", "state-name",
  "label", "state-label_opt", "trans-label", "acc-sig", "acc-sets",
  "state-acc_opt", "trans-acc_opt", "labeled-edges", "some-labeled-edges",
  "incorrectly-unlabeled-edge", "labeled-edge", "state-conj-checked",
  "unlabeled-edges", "unlabeled-edge", "incorrectly-labeled-edge", "dstar",
  "dstar_type", "dstar_header", "dstar_sizes", "dstar_state_id", "sign",
  "dstar_accsigs", "dstar_state_accsig", "dstar_transitions",
  "dstar_states", "never", "$@6", "nc-states", "nc-one-ident",
  "nc-ident-list", "nc-transition-block", "nc-state", "nc-transitions",
  "nc-formula-or-ident", "nc-formula", "nc-opt-dest", "nc-src-dest",
  "nc-transition", "lbtt", "lbtt-header-states", "lbtt-header",
  "lbtt-body", "lbtt-states", "lbtt-state", "lbtt-acc", "lbtt-guard",
  "lbtt-transitions", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   322,   322,   323,   324,   325,   332,   333,   334,   335,
     341,   342,   344,   345,   346,   346,   348,   645,   652,   652,
     655,   654,   684,   684,   685,   703,   708,   712,   713,   725,
     724,   758,   762,   767,   771,   773,   772,   776,   775,   778,
     787,   789,   789,   790,   818,   818,   819,   820,   824,   824,
     846,   863,   863,   867,   867,   872,   872,   873,   874,   878,
     883,   887,   895,   899,   905,   909,   913,   927,   942,   948,
     955,   962,   968,   986,  1014,  1032,  1036,  1042,  1048,  1052,
    1061,  1135,  1145,  1188,  1188,  1227,  1228,  1252,  1262,  1292,
    1297,  1302,  1303,  1325,  1352,  1363,  1368,  1371,  1380,  1383,
    1395,  1398,  1412,  1412,  1413,  1414,  1415,  1416,  1434,  1445,
    1458,  1473,  1474,  1475,  1476,  1507,  1538,  1549,  1550,  1555,
    1567,  1580,  1604,  1605,  1606,  1624,  1637,  1641,  1643,  1683,
    1684,  1688,  1691,  1716,  1718,  1719,  1728,  1729,  1730,  1741,
    1740,  1771,  1773,  1774,  1775,  1777,  1789,  1799,  1817,  1821,
    1827,  1837,  1838,  1839,  1855,  1856,  1866,  1866,  1868,  1898,
    1905,  1908,  1912,  1919,  1938,  1942,  1951,  1992,  1998,  2011,
    2016,  2021,  2022,  2023,  2025,  2044,  2045,  2054,  2094,  2095
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
       2,     2,     2,    32,     2,     2,     2,     2,    31,     2,
      55,    56,     2,    61,     2,    62,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    64,    63,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    57,     2,    58,     2,     2,     2,     2,     2,     2,
       2,     2,    54,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    53,     2,     2,     2,
       2,     2,     2,    59,    30,    60,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52
    };
    const unsigned int user_token_number_max_ = 304;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }


} // hoayy
#line 3877 "parseaut.cc" // lalr1.cc:1167
#line 2114 "parseaut.yy" // lalr1.cc:1168


static void fill_guards(result_& r)
{
  unsigned nap = r.ap.size();

  int* vars = new int[nap];
  for (unsigned i = 0; i < nap; ++i)
    vars[i] = r.ap[nap - 1 - i];

  // build the 2^nap possible guards
  r.guards.reserve(1U << nap);
  for (size_t i = 0; i < (1U << nap); ++i)
    r.guards.push_back(bdd_ibuildcube(i, nap, vars));
  r.cur_guard = r.guards.begin();

  delete[] vars;
}

void
hoayy::parser::error(const location_type& location,
		     const std::string& message)
{
  res.h->errors.emplace_back(location, message);
}

static spot::acc_cond::acc_code
fix_acceptance_aux(spot::acc_cond& acc,
		   spot::acc_cond::acc_code in, unsigned pos,
		   spot::acc_cond::mark_t onlyneg,
		   spot::acc_cond::mark_t both,
		   unsigned base)
{
  auto& w = in[pos];
  switch (w.sub.op)
    {
    case spot::acc_cond::acc_op::And:
      {
	unsigned sub = pos - w.sub.size;
	--pos;
	auto c = fix_acceptance_aux(acc, in, pos, onlyneg, both, base);
	pos -= in[pos].sub.size;
	while (sub < pos)
	  {
	    --pos;
	    c &= fix_acceptance_aux(acc, in, pos, onlyneg, both, base);
	    pos -= in[pos].sub.size;
	  }
	return c;
      }
    case spot::acc_cond::acc_op::Or:
      {
	unsigned sub = pos - w.sub.size;
	--pos;
	auto c = fix_acceptance_aux(acc, in, pos, onlyneg, both, base);
	pos -= in[pos].sub.size;
	while (sub < pos)
	  {
	    --pos;
	    c |= fix_acceptance_aux(acc, in, pos, onlyneg, both, base);
	    pos -= in[pos].sub.size;
	  }
	return c;
      }
    case spot::acc_cond::acc_op::Inf:
      return acc.inf(in[pos - 1].mark);
    case spot::acc_cond::acc_op::Fin:
      return acc.fin(in[pos - 1].mark);
    case spot::acc_cond::acc_op::FinNeg:
      {
	auto m = in[pos - 1].mark;
	auto c = acc.fin(onlyneg & m);
	spot::acc_cond::mark_t tmp = 0U;
	for (auto i: both.sets())
	  {
	    if (m.has(i))
	      tmp.set(base);
	    ++base;
	  }
	if (tmp)
	  c |= acc.fin(tmp);
	return c;
      }
    case spot::acc_cond::acc_op::InfNeg:
      {
	auto m = in[pos - 1].mark;
	auto c = acc.inf(onlyneg & m);
	spot::acc_cond::mark_t tmp = 0U;
	for (auto i: both.sets())
	  {
	    if (m.has(i))
	      tmp.set(base);
	    ++base;
	  }
	if (tmp)
	  c &= acc.inf(tmp);
	return c;
      }
    }
  SPOT_UNREACHABLE();
  return {};
}

static void fix_acceptance(result_& r)
{
  if (r.opts.want_kripke)
    return;
  auto& acc = r.h->aut->acc();

  // If a set x appears only as Inf(!x), we can complement it so that
  // we work with Inf(x) instead.
  auto onlyneg = r.neg_acc_sets - r.pos_acc_sets;
  if (onlyneg)
    {
      for (auto& t: r.h->aut->edge_vector())
	t.acc ^= onlyneg;
    }

  // However if set x is used elsewhere, for instance in
  //   Inf(!x) & Inf(x)
  // complementing x would be wrong.  We need to create a
  // new set, y, that is the complement of x, and rewrite
  // this as Inf(y) & Inf(x).
  auto both = r.neg_acc_sets & r.pos_acc_sets;
  unsigned base = 0;
  if (both)
    {
      base = acc.add_sets(both.count());
      for (auto& t: r.h->aut->edge_vector())
        {
          unsigned i = 0;
	  if ((t.acc & both) != both)
            for (unsigned v : both.sets())
              {
                if (!t.acc.has(v))
                  t.acc |= acc.mark(base + i);
                i++;
              }
        }
    }

  if (onlyneg || both)
    {
      auto& acc = r.h->aut->acc();
      auto code = acc.get_acceptance();
      r.h->aut->set_acceptance(acc.num_sets(),
			       fix_acceptance_aux(acc, code, code.size() - 1,
						  onlyneg, both, base));
    }
}

// Spot only supports a single initial state.
//
// If the input file does not declare any initial state (this is valid
// in the HOA format) add one nonetheless.
//
// If the input file has multiple initial states we have to merge
// them.
//
//   1) In the non-alternating case, this is as simple as making a new
//   initial state using the union of all the outgoing transitions of
//   the declared initial states.  Note that if one of the original
//   initial state has no incoming transition, then we can use it as
//   initial state, avoiding the creation of a new state.
//
//   2) In the alternating case, the input may have several initial
//   states that are conjuncts.  We have to reduce the conjuncts to a
//   single state first.

static void fix_initial_state(result_& r)
{
  std::vector<std::vector<unsigned>> start;
  start.reserve(r.start.size());
  unsigned ssz = r.info_states.size();
  for (auto& p : r.start)
    {
      std::vector<unsigned> v;
      v.reserve(p.second.size());
      for (unsigned s: p.second)
        // Ignore initial states without declaration
        // (They have been diagnosed already.)
        if (s < ssz && r.info_states[s].declared)
          v.emplace_back(s);
      if (!v.empty())
        start.push_back(v);
    }

  // If no initial state has been declared, add one.
  if (start.empty())
    {
      if (r.opts.want_kripke)
	r.h->ks->set_init_state(r.h->ks->new_state(bddfalse));
      else
	r.h->aut->set_init_state(r.h->aut->new_state());
      return;
    }

  // Remove any duplicate initial state.
  std::sort(start.begin(), start.end());
  auto res = std::unique(start.begin(), start.end());
  start.resize(std::distance(start.begin(), res));

  assert(start.size() >= 1);
  if (start.size() == 1)
    {
      if (r.opts.want_kripke)
	r.h->ks->set_init_state(start.front().front());
      else
	r.h->aut->set_univ_init_state(start.front().begin(),
                                      start.front().end());
    }
  else
    {
      if (r.opts.want_kripke)
	{
	  r.h->errors.emplace_front(r.start.front().first,
				    "Kripke structure only support "
				    "a single initial state");
	  return;
	}
      // Fiddling with initial state may turn an incomplete automaton
      // into a complete one.
      if (r.complete.is_false())
        r.complete = spot::trival::maybe();
      // Multiple initial states.  We might need to add a fake one,
      // unless one of the actual initial state has no incoming edge.
      auto& aut = r.h->aut;
      std::vector<unsigned char> has_incoming(aut->num_states(), 0);
      for (auto& t: aut->edges())
        for (unsigned ud: aut->univ_dests(t))
          has_incoming[ud] = 1;

      bool found = false;
      unsigned init = 0;
      bool init_alternation = false;
      for (auto& pp: start)
        if (pp.size() == 1)
          {
            unsigned p = pp.front();
            if (!has_incoming[p])
              {
                init = p;
                found = true;
              }
          }
        else
          {
            init_alternation = true;
            break;
          }

      if (!found || init_alternation)
	// We do need a fake initial state
	init = aut->new_state();
      aut->set_init_state(init);

      // The non-alternating case is the easiest, we simply declare
      // the outgoing transitions of all "original initial states"
      // into the only one initial state.
      if (!init_alternation)
        {
          for (auto& pp: start)
            {
              unsigned p = pp.front();
              if (p != init)
                for (auto& t: aut->out(p))
                  aut->new_edge(init, t.dst, t.cond);
            }
        }
      else
        {
          // In the alternating case, we merge outgoing transition of
          // the universal destination of conjunct initial states.
          // (Note that this loop would work for the non-alternating
          // case too, but it is more expansive, so we avoid it if we
          // can.)
          spot::outedge_combiner combiner(aut);
          bdd comb_or = bddfalse;
          for (auto& pp: start)
            {
              bdd comb_and = bddtrue;
              for (unsigned d: pp)
                comb_and &= combiner(d);
              comb_or |= comb_and;
            }
          combiner.new_dests(init, comb_or);
        }
    }
}

static void fix_properties(result_& r)
{
  r.aut_or_ks->prop_universal(r.universal);
  r.aut_or_ks->prop_complete(r.complete);
  if (r.acc_style == State_Acc ||
      (r.acc_style == Mixed_Acc && !r.trans_acc_seen))
    r.aut_or_ks->prop_state_acc(true);
}

static void check_version(const result_& r)
{
  if (r.h->type != spot::parsed_aut_type::HOA)
    return;
  auto& v = r.format_version;
  if (v.size() < 2 || v[0] != 'v' || v[1] < '1' || v[1] > '9')
    {
      r.h->errors.emplace_front(r.format_version_loc, "unknown HOA version");
      return;
    }
  const char* beg = &v[1];
  char* end = nullptr;
  long int vers = strtol(beg, &end, 10);
  if (vers != 1)
    {
      r.h->errors.emplace_front(r.format_version_loc,
				  "unsupported HOA version");
      return;
    }
  constexpr const char supported[] = "1";
  if (strverscmp(supported, beg) < 0 && !r.h->errors.empty())
    {
      std::ostringstream s;
      s << "we can read HOA v" << supported
	<< " but this file uses " << v << "; this might "
	"cause the following errors";
      r.h->errors.emplace_front(r.format_version_loc, s.str());
      return;
    }
}

namespace spot
{
  automaton_stream_parser::automaton_stream_parser(const std::string& name,
						   automaton_parser_options opt)
  try
    : filename_(name), opts_(opt)
  {
    if (hoayyopen(name, &scanner_))
      throw std::runtime_error(std::string("Cannot open file ") + name);
  }
  catch (...)
  {
    hoayyclose(scanner_);
    throw;
  }

  automaton_stream_parser::automaton_stream_parser(int fd,
						   const std::string& name,
						   automaton_parser_options opt)
  try
    : filename_(name), opts_(opt)
  {
    if (hoayyopen(fd, &scanner_))
      throw std::runtime_error(std::string("Cannot open file ") + name);
  }
  catch (...)
  {
    hoayyclose(scanner_);
    throw;
  }

  automaton_stream_parser::automaton_stream_parser(const char* data,
						   const std::string& filename,
						   automaton_parser_options opt)
  try
    : filename_(filename), opts_(opt)
  {
    hoayystring(data, &scanner_);
  }
  catch (...)
  {
    hoayyclose(scanner_);
    throw;
  }

  automaton_stream_parser::~automaton_stream_parser()
  {
    hoayyclose(scanner_);
  }

  static void raise_parse_error(const parsed_aut_ptr& pa)
  {
    if (pa->aborted)
      pa->errors.emplace_back(pa->loc, "parsing aborted");
    if (!pa->errors.empty())
      {
	std::ostringstream s;
	if (pa->format_errors(s))
	  throw parse_error(s.str());
      }
    // It is possible that pa->aut == nullptr if we reach the end of a
    // stream.  It is not necessarily an error.
  }

  parsed_aut_ptr
  automaton_stream_parser::parse(const bdd_dict_ptr& dict,
				 environment& env)
  {
  restart:
    result_ r;
    r.opts = opts_;
    r.h = std::make_shared<spot::parsed_aut>(filename_);
    if (opts_.want_kripke)
      r.aut_or_ks = r.h->ks = make_kripke_graph(dict);
    else
      r.aut_or_ks = r.h->aut = make_twa_graph(dict);
    r.env = &env;
    hoayy::parser parser(scanner_, r, last_loc);
    static bool env_debug = !!getenv("SPOT_DEBUG_PARSER");
    parser.set_debug_level(opts_.debug || env_debug);
    hoayyreset(scanner_);
    try
      {
	if (parser.parse())
	  {
	    r.h->aut = nullptr;
	    r.h->ks = nullptr;
	    r.aut_or_ks = nullptr;
	  }
      }
    catch (const spot::hoa_abort& e)
      {
	r.h->aborted = true;
	// Bison 3.0.2 lacks a += operator for locations.
	r.h->loc = r.h->loc + e.pos;
      }
    check_version(r);
    last_loc = r.h->loc;
    last_loc.step();
    if (r.h->aborted)
      {
	if (opts_.ignore_abort)
	  goto restart;
	return r.h;
      }
    if (opts_.raise_errors)
      raise_parse_error(r.h);
    if (!r.aut_or_ks)
      return r.h;
    if (r.state_names)
      r.aut_or_ks->set_named_prop("state-names", r.state_names);
    if (r.highlight_edges)
      r.aut_or_ks->set_named_prop("highlight-edges", r.highlight_edges);
    if (r.highlight_states)
      r.aut_or_ks->set_named_prop("highlight-states", r.highlight_states);
    fix_acceptance(r);
    fix_initial_state(r);
    fix_properties(r);
    if (r.h->aut && !r.h->aut->is_existential())
      r.h->aut->merge_univ_dests();
    return r.h;
  }

  parsed_aut_ptr
  parse_aut(const std::string& filename, const bdd_dict_ptr& dict,
	    environment& env, automaton_parser_options opts)
  {
    auto localopts = opts;
    localopts.raise_errors = false;
    parsed_aut_ptr pa;
    try
      {
	automaton_stream_parser p(filename, localopts);
	pa = p.parse(dict, env);
      }
    catch (std::runtime_error& e)
      {
	if (opts.raise_errors)
	  throw;
	parsed_aut_ptr pa = std::make_shared<spot::parsed_aut>(filename);
	pa->errors.emplace_back(spot::location(), e.what());
	return pa;
      }
    if (!pa->aut && !pa->ks && pa->errors.empty())
      pa->errors.emplace_back(pa->loc, "no automaton read (empty input?)");
    if (opts.raise_errors)
      raise_parse_error(pa);
    return pa;
  }


}

// Local Variables:
// mode: c++
// End:
