/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    PROGRAMnum = 258,              /* PROGRAMnum  */
    IDnum = 259,                   /* IDnum  */
    SEMInum = 260,                 /* SEMInum  */
    CLASSnum = 261,                /* CLASSnum  */
    DECLARATIONSnum = 262,         /* DECLARATIONSnum  */
    ENDDECLARATIONSnum = 263,      /* ENDDECLARATIONSnum  */
    COMMAnum = 264,                /* COMMAnum  */
    EQUALnum = 265,                /* EQUALnum  */
    LBRACEnum = 266,               /* LBRACEnum  */
    RBRACEnum = 267,               /* RBRACEnum  */
    LBRACnum = 268,                /* LBRACnum  */
    RBRACnum = 269,                /* RBRACnum  */
    LPARENnum = 270,               /* LPARENnum  */
    RPARENnum = 271,               /* RPARENnum  */
    VOIDnum = 272,                 /* VOIDnum  */
    INTnum = 273,                  /* INTnum  */
    METHODnum = 274,               /* METHODnum  */
    VALnum = 275,                  /* VALnum  */
    DOTnum = 276,                  /* DOTnum  */
    ASSGNnum = 277,                /* ASSGNnum  */
    RETURNnum = 278,               /* RETURNnum  */
    IFnum = 279,                   /* IFnum  */
    ELSEnum = 280,                 /* ELSEnum  */
    WHILEnum = 281,                /* WHILEnum  */
    LTnum = 282,                   /* LTnum  */
    LEnum = 283,                   /* LEnum  */
    EQnum = 284,                   /* EQnum  */
    NEnum = 285,                   /* NEnum  */
    GEnum = 286,                   /* GEnum  */
    GTnum = 287,                   /* GTnum  */
    PLUSnum = 288,                 /* PLUSnum  */
    MINUSnum = 289,                /* MINUSnum  */
    ORnum = 290,                   /* ORnum  */
    TIMESnum = 291,                /* TIMESnum  */
    DIVIDEnum = 292,               /* DIVIDEnum  */
    ANDnum = 293,                  /* ANDnum  */
    NOTnum = 294,                  /* NOTnum  */
    ICONSTnum = 295,               /* ICONSTnum  */
    SCONSTnum = 296                /* SCONSTnum  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define PROGRAMnum 258
#define IDnum 259
#define SEMInum 260
#define CLASSnum 261
#define DECLARATIONSnum 262
#define ENDDECLARATIONSnum 263
#define COMMAnum 264
#define EQUALnum 265
#define LBRACEnum 266
#define RBRACEnum 267
#define LBRACnum 268
#define RBRACnum 269
#define LPARENnum 270
#define RPARENnum 271
#define VOIDnum 272
#define INTnum 273
#define METHODnum 274
#define VALnum 275
#define DOTnum 276
#define ASSGNnum 277
#define RETURNnum 278
#define IFnum 279
#define ELSEnum 280
#define WHILEnum 281
#define LTnum 282
#define LEnum 283
#define EQnum 284
#define NEnum 285
#define GEnum 286
#define GTnum 287
#define PLUSnum 288
#define MINUSnum 289
#define ORnum 290
#define TIMESnum 291
#define DIVIDEnum 292
#define ANDnum 293
#define NOTnum 294
#define ICONSTnum 295
#define SCONSTnum 296

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 18 "grammar.y"

  int intg;
  tree tptr;
  

#line 155 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
