/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2009  Fabian Schmitthenner

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

#ifndef CAS_OPERATOR_H
#define CAS_OPERATOR_H

#include "term.h"
#include "termreference.h"
#include <map>
#include <vector>
#include "termcollection.h"
#include <gmpxx.h>

namespace CAS {
class BuildInFunction;

class Operator : public CAS::Term
{
  protected:
    typedef mpq_class NumberX;
    TermCollectionTemplate<NumberX> children;
    ///Die FindEqual-Funktion darf KEINE Änderungen an children durchführen, die Iteratoren ungültig machen; diese Änderungen sollten
    ///ans Ende angestellt werden!
    void FindEquals (void (Operator::*) (CAS::TermReference*, int));
    Operator ();
    Operator (const TermCollectionTemplate<NumberX>& c);
    Hash GetPseudoHashCode (CAS::hashes::Hashes hT1, uint32_t data) const;
    void PseudoToString (std::ostream &stream, const std::string &op) const;
    
  public:
    template<class C, class _It> 
    void Where (_It output_iterator, bool (Operator::*predicate) (TermCollectionTemplate<NumberX>::iterator h))
    {
      for (TermCollectionTemplate<NumberX>::iterator it = children.begin(); it != children.end();)
      {
	const C *c = dynamic_cast<const C *> (it->second.first->get_const ());
	if (!c)
	{
	  ++it;
	  continue;
	}
	if ((this->*predicate) (it))
	{
	  *output_iterator++ = it->second;
	  children.erase(it++);
	  continue;
	}
	++it;
      }
    }
    
  public:
    bool True (TermCollectionTemplate<NumberX>::iterator c)
    {
      return true;
    }
    
  protected:
    template<class C>
    bool SimplifyEx ()
    {
      std::vector< std::pair< TermReference *, NumberX > > refs;
      Where< C > (std::back_inserter(refs), &Operator::True);
      for (std::vector< std::pair< TermReference*, NumberX > >::const_iterator it = refs.begin(); it != refs.end(); ++it)
      {
	const Operator *op = static_cast< const Operator * > (it->first->get_const());
	for (TermCollectionTemplate<NumberX>::const_iterator it2 = op->children.begin(); it2 != op->children.end(); ++it2)
	{
	  //TODO:BUG: für multiplikation muss es statt xx * yy heißen: xx ^ yy (!!)
	  // kritischer Fehler!!
	  push_back (std::make_pair(it2->second.first->clone(), it2->second.second * it->second));
	}
	delete it->first;
      }
      return !refs.empty();
    }
    
    ///Vereinfachungsmethoden:
    std::pair<TermReference *, NumberX> GetSingleObject ();
    
    virtual TermReference *GetElement(TermCollectionTemplate<NumberX>::const_iterator arg1) const = 0;
    virtual void push_back(std::pair< TermReference*, NumberX > arg1) = 0;
  public:
//     virtual CAS::Type* GetType() const;
    virtual TermReference *simplify();
    virtual ~Operator();
    virtual bool equals(const CAS::Term& t) const;
    virtual TermReference* getChildren(void*& param) const;
    virtual TermReference* getChildrenVar(void*& param) const;
    
    //friend TermReference* OperatorRule::MatchRule(const TermReference* t, std::vector< TermReference* >::iterator params, int count) const;
};

class Add: public Operator
{
  private:
    Add ();
    Add (const Add &a);
    Add (CAS::TermReference** t, size_t anz);
    void push_back(TermReference* arg1);
    virtual void push_back(std::pair< TermReference*, NumberX > arg1);
    virtual TermReference* GetElement(std::multimap< CAS::Hash, std::pair< CAS::TermReference*, CAS::Operator::NumberX > >::const_iterator arg1) const;
  public:
    virtual Term* clone() const;
    virtual Hash getHashCode() const;
    virtual void toString(std::ostream& stream) const;
    virtual TermReference *simplify();
    virtual bool equals(const CAS::Term& t) const;
    virtual Term* CreateTerm(TermReference** children) const;
    static Add *CreateTerm (CAS::TermReference* t1, CAS::TermReference* t2);
    static Add *CreateTerm (CAS::TermReference* t1, CAS::TermReference* t2, CAS::TermReference *t3);
    static Add *CreateTerm (TermReference **children, size_t anzahl);
};

class Mul: public Operator
{
  private:
    Mul (const Mul &m);
    Mul ();
    Mul (CAS::TermReference** t, size_t anz);
    virtual void push_back(std::pair< TermReference*, NumberX > arg1);
    void push_back(TermReference* arg1);
    virtual TermReference* GetElement(std::multimap< Hash, std::pair< TermReference*, NumberX > >::const_iterator arg1) const;
    bool ShouldChoose (TermCollectionTemplate<NumberX>::iterator it)
    {
      return it->second.second >= 0;
    }
  public:
    virtual Term* clone() const;
    virtual Hash getHashCode() const;
    virtual bool equals(const CAS::Term& t) const;
    virtual void toString(std::ostream& stream) const;
    static Mul *CreateTerm (CAS::TermReference* t1, CAS::TermReference* t2);
    static Mul *CreateTerm (CAS::TermReference* t1, CAS::TermReference* t2, CAS::TermReference *t3);
    static Mul *CreateTerm (CAS::TermReference **children, size_t anzahl);
    virtual Term* CreateTerm(CAS::TermReference** children) const;
    virtual TermReference* simplify();
};

}

#endif // CAS_OPERATOR_H
