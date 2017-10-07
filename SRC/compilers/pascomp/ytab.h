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
    SSCANF = 265,
    SPRINTF = 266,
    IDENTIFIER = 267,
    CHAR = 268,
    INT = 269,
    NE = 270,
    LE = 271,
    GE = 272,
    BECOMES = 273,
    DIV = 274,
    MOD = 275,
    OR = 276,
    AND = 277,
    NOT = 278,
    DOTDOT = 279,
    IF = 280,
    THEN = 281,
    ELSE = 282,
    CASE = 283,
    OF = 284,
    REPEAT = 285,
    UNTIL = 286,
    WHILE = 287,
    DO = 288,
    FOR = 289,
    TO = 290,
    CBEGIN = 291,
    SBEGIN = 292,
    END = 293,
    CEND = 294,
    CONST = 295,
    VAR = 296,
    TYPE = 297,
    ARRAY = 298,
    FUNCTION = 299,
    MONITOR = 300,
    PROCEDURE = 301,
    PROGRAM = 302,
    INCLUDE = 303,
    ATOMIC = 304,
    FNSTRING = 305,
    SEND = 306,
    RECEIVE = 307,
    BROADCAST = 308,
    EXTERNAL = 309,
    LOWER_THAN_ELSE = 310
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
#define SSCANF 265
#define SPRINTF 266
#define IDENTIFIER 267
#define CHAR 268
#define INT 269
#define NE 270
#define LE 271
#define GE 272
#define BECOMES 273
#define DIV 274
#define MOD 275
#define OR 276
#define AND 277
#define NOT 278
#define DOTDOT 279
#define IF 280
#define THEN 281
#define ELSE 282
#define CASE 283
#define OF 284
#define REPEAT 285
#define UNTIL 286
#define WHILE 287
#define DO 288
#define FOR 289
#define TO 290
#define CBEGIN 291
#define SBEGIN 292
#define END 293
#define CEND 294
#define CONST 295
#define VAR 296
#define TYPE 297
#define ARRAY 298
#define FUNCTION 299
#define MONITOR 300
#define PROCEDURE 301
#define PROGRAM 302
#define INCLUDE 303
#define ATOMIC 304
#define FNSTRING 305
#define SEND 306
#define RECEIVE 307
#define BROADCAST 308
#define EXTERNAL 309
#define LOWER_THAN_ELSE 310

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
