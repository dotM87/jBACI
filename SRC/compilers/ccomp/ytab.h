/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    UNSIGNED_INT = 258,
    STRING = 259,
    RAWSTRING = 260,
    STRINGCONCAT = 261,
    STRINGCOMPARE = 262,
    STRINGCOPY = 263,
    STRINGLENGTH = 264,
    IDENTIFIER = 265,
    CHARCON = 266,
    INCLUDE = 267,
    FNSTRING = 268,
    NE = 269,
    LE = 270,
    GE = 271,
    EQ = 272,
    OR = 273,
    AND = 274,
    PLPL = 275,
    MIMI = 276,
    IF = 277,
    ELSE = 278,
    SWITCH = 279,
    CASE = 280,
    DEFAULT = 281,
    WHILE = 282,
    DO = 283,
    FOR = 284,
    CBEGIN = 285,
    CONST = 286,
    TYPEDEF = 287,
    VOID = 288,
    INT = 289,
    CHAR = 290,
    CONTINUE = 291,
    BREAK = 292,
    RETURN = 293,
    COUT = 294,
    LTLT = 295,
    ENDL = 296,
    CIN = 297,
    GTGT = 298,
    MAIN = 299,
    MONITOR = 300,
    INIT = 301,
    ATOMIC = 302,
    SEND = 303,
    RECEIVE = 304,
    BROADCAST = 305,
    EXTERN = 306,
    SSCANF = 307,
    SPRINTF = 308,
    LOWER_THAN_ELSE = 309
  };
#endif
/* Tokens.  */
#define UNSIGNED_INT 258
#define STRING 259
#define RAWSTRING 260
#define STRINGCONCAT 261
#define STRINGCOMPARE 262
#define STRINGCOPY 263
#define STRINGLENGTH 264
#define IDENTIFIER 265
#define CHARCON 266
#define INCLUDE 267
#define FNSTRING 268
#define NE 269
#define LE 270
#define GE 271
#define EQ 272
#define OR 273
#define AND 274
#define PLPL 275
#define MIMI 276
#define IF 277
#define ELSE 278
#define SWITCH 279
#define CASE 280
#define DEFAULT 281
#define WHILE 282
#define DO 283
#define FOR 284
#define CBEGIN 285
#define CONST 286
#define TYPEDEF 287
#define VOID 288
#define INT 289
#define CHAR 290
#define CONTINUE 291
#define BREAK 292
#define RETURN 293
#define COUT 294
#define LTLT 295
#define ENDL 296
#define CIN 297
#define GTGT 298
#define MAIN 299
#define MONITOR 300
#define INIT 301
#define ATOMIC 302
#define SEND 303
#define RECEIVE 304
#define BROADCAST 305
#define EXTERN 306
#define SSCANF 307
#define SPRINTF 308
#define LOWER_THAN_ELSE 309

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
