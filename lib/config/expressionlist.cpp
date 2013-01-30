/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012 Icinga Development Team (http://www.icinga.org/)        *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#include "i2-config.h"

using namespace icinga;

/**
 * Adds an expression to an expression list.
 *
 * @param expression The expression that should be added.
 */
void ExpressionList::AddExpression(const Expression& expression)
{
	m_Expressions.push_back(expression);
}

/**
 * Returns the number of items currently contained in the expression list.
 *
 * @returns The length of the list.
 */
size_t ExpressionList::GetLength(void) const
{
	return m_Expressions.size();
}

/**
 * Executes the expression list.
 *
 * @param dictionary The dictionary that should be manipulated by the
 *		     expressions.
 */
void ExpressionList::Execute(const Dictionary::Ptr& dictionary) const
{
	BOOST_FOREACH(const Expression& expression, m_Expressions) {
		expression.Execute(dictionary);
	}
}

/**
 * Dumps the expression list to the specified stream.
 *
 * @param fp The stream.
 * @param indent The indentation level.
 */
void ExpressionList::Dump(ostream& fp, int indent) const
{
	BOOST_FOREACH(const Expression& expression, m_Expressions) {
		expression.Dump(fp, indent);
	}
}