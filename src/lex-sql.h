/**
 * 
 *  This file is part of apidb.
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * */

enum Token
{
	TYPE_BIT,
	
	TYPE_INT_BYTES1,
	TYPE_INT_BYTES2,
	TYPE_INT_BYTES3,
	TYPE_INT_BYTES4,
	TYPE_INT_BYTES8,
	TYPE_INT_BYTES16,
	TYPE_INT_WORD,
	
	TYPE_DECIMAL,
	TYPE_FLOAT,
	TYPE_DOUBLE,
	
	TYPE_CHAR, /* char */
	TYPE_STRING,/* const char* */
	
	SPACE,
	
	COMA,
	PARENTHESIS_OPEN,
	PARENTHESIS_CLOSE,
	NUMBER,
	WORD,
	UNKNOW,
	END
}; 
