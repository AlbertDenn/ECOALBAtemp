/*
  Copyright (C) 2014
  Alejandro Mujica (amujica@cenditel.gob.ve)
  José Ruiz (jruiz@cenditel.gob.ve)
  Julie Vera (jvera@cenditel.gob.ve)
 
  CENDITEL Fundación Centro Nacional de Desarrollo e Investigación en
  Tecnologías Libres
 
  Este programa es software libre; Usted puede usarlo bajo los términos de la
  licencia de software GPL versión 2.0 de la Free Software Foundation.
 
  Este programa se distribuye con la esperanza de que sea útil, pero SIN
  NINGUNA GARANTÍA; tampoco las implícitas garantías de MERCANTILIDAD o
  ADECUACIÓN A UN PROPÓSITO PARTICULAR.
  Consulte la licencia GPL para más detalles. Usted debe recibir una copia
  de la GPL junto con este programa; si no, escriba a la Free Software
  Foundation Inc. 51 Franklin Street,5 Piso, Boston, MA 02110-1301, USA.
*/

/*
  Autor:             Alejandro J. Mujica
  Fecha de creación: 09/06/2014
  Este archivo contiene la implementación de la abstracción de empresa o unidad
  económica.
*/

# include <company.H>
Company::Company()
  : rif(), name(), working_day(0)
{

}

Company::Company(const Company & company)
  : rif(company.rif), name(company.name), working_day(company.working_day)
{
  // Empty
}

Company::Company(Company && company)
  : rif(), name(), working_day(0)
{
  std::swap(rif, company.rif);
  std::swap(name, company.name);
  std::swap(working_day, company.working_day);
}

const std::string & Company::get_rif() const
{
  return rif;
}

void Company::set_rif(const std::string & _rif)
{
  rif = _rif;
}

void Company::set_rif(std::string && _rif)
{
  rif = std::move(_rif);
}

const std::string & Company::get_name() const
{
  return name;
}

void Company::set_name(const std::string & _name)
{
  name = _name;
}

void Company::set_name(std::string && _name)
{
  name = std::move(_name);
}

const real & Company::get_working_day() const
{
  return working_day;
}

void Company::set_working_day(const real & _working_day)
{
  working_day = _working_day;
}

Company & Company::operator = (const Company & company)
{
  if (&company == this)
    return *this;

  rif = company.rif;
  name = company.name;
  working_day = company.working_day;

  return *this;
}

Company & Company::operator = (Company && company)
{
  std::swap(rif, company.rif);
  std::swap(name, company.name);
  std::swap(working_day, company.working_day);

  return *this;
}

