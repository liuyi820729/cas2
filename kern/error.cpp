#include "error.h"
#include "termreference.h"


void CAS::Unknown::ToString(std::ostream& stream) const
{
  stream << "Unknown";
}

CAS::Term* CAS::Limit::Clone() const
{
  return new Limit (argument);
}

CAS::Term* CAS::Limit::CreateTerm(CAS::TermReference** children) const
{
  return new Limit (*children);
}

bool CAS::Limit::Equals(const CAS::Term& t) const
{
  return false;
}

CAS::TermReference* CAS::Limit::GetChildrenVar(void*& param) const
{
  if (param == 0)
  {
    param = (void *)1;
    return argument;
  }
  else
    return NULL;
}

CAS::Hash CAS::Limit::GetHashCode() const
{
  return Hash (hashes::Limit, 0) ^ argument->GetHashCode();
}

CAS::Type* CAS::Limit::GetType() const
{
  return Type::GetBuildInType(Type::Term);
}

CAS::TermReference* CAS::Limit::Simplify()
{
  return coll->Simplify(this);
}

void CAS::Limit::ToString(std::ostream& stream) const
{
  stream << "Infinity*(e^(i*" << *argument << "))";
}





