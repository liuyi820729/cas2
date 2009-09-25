/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#ifndef CAS_RULE_H
#define CAS_RULE_H
#include "type.h"
#include <vector>

namespace CAS {
class Term;

class Rule
{
  public:
    virtual Type *GetCorrespondingType () const = 0;
    //gibt NULL zurück, falls die Regel nicht angewandt werden konnte; sonst gibt es ein (weiteres) Term * zurück
    virtual Term *UseRule (const Term *) const = 0;
};

class SubRule: public Rule
{
  private:
    size_t parameterCount;
  public:
    virtual Term *MatchRule (const Term *t, std::vector< Term * >::iterator params, int count) const = 0;
    virtual Term* UseRule(const Term *t) const;
};

class OperatorRule: public SubRule
{
  private:
    std::vector< SubRule * > children;
  public:
    virtual Type* GetCorrespondingType() const;
    virtual Term* MatchRule(const CAS::Term* t, std::vector< CAS::Term* >::iterator params, int count) const;    
};

}

#endif // CAS_RULE_H
