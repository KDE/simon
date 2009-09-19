/*
 *   Copyright (C) 2009 Dominik Neumeister <domar@gmx.at>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "token.h"

Token::Token(char arOp, short priority)
    : arOperator(arOp),
    type(priority),
    number(0)
{
}

Token::Token(double num) : number(num), type(0), arOperator(' ')
{
}
short Token::getType()
{
    return type;
}

char Token::getArOperator()
{
    return arOperator;
}

double Token::getNumber()
{
    return number;
}
