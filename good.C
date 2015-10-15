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
  Fecha de creación: 05/06/2014
  Este archivo contiene la implementación de la abstracción de un Bien.
*/

# include "good.H"

Good::Good()
  : company_rif(), company_name(), company_location(), nationality("N"), id(0),
    name(), tariff_code(), measurement_unit(), level(0)
{
  // Empty
}

Good::Good(const Good & good)
  : company_rif(good.company_rif), company_name(good.company_name),
    company_location(good.company_location), nationality(good.nationality),
    id(good.id), name(good.name), tariff_code(good.tariff_code),
    measurement_unit(good.measurement_unit), level(good.level)
{
  // Empty
}

Good::Good(Good && good)
  : company_rif(), company_name(), company_location(), nationality("N"), id(0),
    name(), tariff_code(), measurement_unit(), level(0)
{
  std::swap(company_rif, good.company_rif);
  std::swap(company_name, good.company_name);
  std::swap(company_location, good.company_location);
  std::swap(nationality, good.nationality);
  std::swap(id, good.id);
  std::swap(name, good.name);
  std::swap(tariff_code, good.tariff_code);
  std::swap(measurement_unit, good.measurement_unit);
  std::swap(level, good.level);
}

const std::string & Good::get_company_rif() const
{
  return company_rif;
}

void Good::set_company_rif(const std::string & _company_rif)
{
  company_rif = _company_rif;
}

void Good::set_company_rif(std::string && _company_rif)
{
  company_rif = std::move(_company_rif);
}

const std::string & Good::get_company_name() const
{
  return company_name;
}

void Good::set_company_name(const std::string & _company_name)
{
  company_name = _company_name;
}

void Good::set_company_name(std::string && _company_name)
{
  company_name = std::move(_company_name);
}

const std::string & Good::get_company_location() const
{
  return company_location;
}

void Good::set_company_location(const std::string & _company_location)
{
  company_location = _company_location;
}

void Good::set_company_location(std::string && _company_location)
{
  company_location = std::move(_company_location);
}

const std::string & Good::get_nationality() const
{
  return nationality;
}

void Good::set_nationality(const std::string & _nationality)
{
  nationality = _nationality;
}

void Good::set_nationality(std::string && _nationality)
{
  nationality = std::move(_nationality);
}


const db_id_t & Good::get_id() const
{
  return id;
}

void Good::set_id(const db_id_t & _id)
{
  id = _id;
}

const std::string & Good::get_name() const
{
  return name;
}

void Good::set_name(const std::string & _name)
{
  name = _name;
}

void Good::set_name(std::string && _name)
{
  name = std::move(_name);
}

const std::string & Good::get_tariff_code() const
{
  return tariff_code;
}

void Good::set_tariff_code(const std::string & _tariff_code)
{
  tariff_code = _tariff_code;
}

void Good::set_tariff_code(std::string && _tariff_code)
{
  tariff_code = std::move(_tariff_code);
}

const std::string & Good::get_measurement_unit() const
{
  return measurement_unit;
}

void Good::set_measurement_unit(const std::string & _measurement_unit)
{
  measurement_unit = _measurement_unit;
}

void Good::set_measurement_unit(std::string && _measurement_unit)
{
  measurement_unit = std::move(_measurement_unit);
}

const int & Good::get_level() const
{
  return level;
}

void Good::set_level(const int & _level)
{
  level = _level;
}

Good & Good::operator = (const Good & good)
{
  if (&good == this)
    return *this;

  company_rif = good.company_rif;
  company_name = good.company_name;
  nationality = good.nationality;
  id = good.id;
  name = good.name;
  tariff_code = good.tariff_code;
  measurement_unit = good.measurement_unit;
  level = good.level;

  return *this;
}

Good & Good::operator = (Good && good)
{
  std::swap(company_rif, good.company_rif);
  std::swap(company_name, good.company_name);
  std::swap(nationality, good.nationality);
  std::swap(id, good.id);
  std::swap(name, good.name);
  std::swap(tariff_code, good.tariff_code);
  std::swap(measurement_unit, good.measurement_unit);
  std::swap(level, good.level);

  return *this;
}

std::string Good::to_dot() const
{
  std::stringstream sstr;

  sstr << "RIF: " << company_rif << "\\n"
       << "Empresa: " << company_name << "\\n"
       << "Nac: " << nationality << "\\n"
       << "ID: " << id << "\\n"
       << "Producto: " << name << "\\n"
       << "Cod Aran: " << tariff_code << "\\n"
       << "Unidad Medida: " << measurement_unit << "\\n"
       << "Nivel: " << level;

  return sstr.str();
}

