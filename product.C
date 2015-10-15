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
  Este archivo contiene la implementación de la clase Product.
*/

# include "product.H"

Product::Product()
  : Good(), production_capacity(0.0), production(0.0),
    internal_sales(0.0), external_sales(0.0), workday(0.0),
    num_administrative_staff(0.0), num_employees(0.0), price(0.0),
    other_cost(0.0)
{
  // Empty
}

Product::Product(const Product & product)
  : Good(product), production_capacity(product.production_capacity),
    production(product.production), internal_sales(product.internal_sales),
    external_sales(product.external_sales), workday(product.workday),
    num_administrative_staff(product.num_administrative_staff),
    num_employees(product.num_employees), price(product.price),
    other_cost(product.other_cost)
{
  // Empty
}

Product::Product(Product && product)
  : Good(std::move(product)), production_capacity(0.0), production(0.0),
    internal_sales(0.0), external_sales(0.0), workday(0.0),
    num_administrative_staff(0.0), num_employees(0.0), price(0.0),
    other_cost(0.0)
{
  std::swap(production_capacity, product.production_capacity);
  std::swap(production, product.production);
  std::swap(internal_sales, product.internal_sales);
  std::swap(external_sales, product.external_sales);
  std::swap(workday, product.workday);
  std::swap(num_administrative_staff, product.num_administrative_staff);
  std::swap(num_employees, product.num_employees);
  std::swap(price, product.price);
  std::swap(other_cost, product.other_cost);

}

const real & Product::get_production_capacity() const
{
  return production_capacity;
}

void Product::set_production_capacity(const real & _production_capacity)
{
  production_capacity = _production_capacity;
}

const real & Product::get_production() const
{
  return production;
}

void Product::set_production(const real & _production)
{
  production = _production;
}

const real & Product::get_internal_sales() const
{
  return internal_sales;
}

void Product::set_internal_sales(const real & _internal_sales)
{
  internal_sales = _internal_sales;
}

const real & Product::get_external_sales() const
{
  return external_sales;
}

void Product::set_external_sales(const real & _external_sales)
{
  external_sales = _external_sales;
}

const real & Product::get_workday() const
{
  return workday;
}

void Product::set_workday(const real & _workday)
{
  workday = _workday;
}

const real & Product::get_num_administrative_staff() const
{
  return num_administrative_staff;
}

void Product::set_num_administrative_staff(
  const real & _num_administrative_staff)
{
  num_administrative_staff = _num_administrative_staff;
}

const real & Product::get_num_employees() const
{
  return num_employees;
}

void Product::set_num_employees(const real & _num_employees)
{
  num_employees = _num_employees;
}

const real & Product::get_price() const
{
  return price;
}

void Product::set_price(const real & _price)
{
  price = _price;
}

const real & Product::get_other_cost() const
{
  return other_cost;
}

void Product::set_other_cost(const real & _other_cost)
{
  other_cost = _other_cost;
}

real Product::get_relationship_manhours() const
{
  return num_employees * workday * WORKED_DAYS_IN_A_YEAR / production;
}

Product & Product::operator = (const Product & product)
{
  if (&product == this)
    return *this;

  (Good &) *this = product;

  production_capacity = product.production_capacity;
  production = product.production;
  internal_sales = product.internal_sales;
  external_sales = product.external_sales;
  workday = product.workday;
  num_administrative_staff = product.num_administrative_staff;
  num_employees = product.num_employees;
  price = product.price;
  other_cost = product.other_cost;


  return *this;
}

Product & Product::operator = (Product && product)
{
  (Good &) *this = std::move(product);

  std::swap(production_capacity, product.production_capacity);
  std::swap(production, product.production);
  std::swap(internal_sales, product.internal_sales);
  std::swap(external_sales, product.external_sales);
  std::swap(workday, product.workday);
  std::swap(num_administrative_staff, product.num_administrative_staff);
  std::swap(num_employees, product.num_employees);
  std::swap(price, product.price);
  std::swap(other_cost, product.other_cost);


  return *this;
}

std::string Product::to_dot() const
{
  std::stringstream sstr;

  sstr << Good::to_dot() << "\\n"
       << "Capacidad: " << production_capacity << "\\n"
       << "Produccion: " << production << "\\n"
       << "Ventas Internas: " << internal_sales << "\\n"
       << "Ventas Externas: " << external_sales << "\\n"
       << "Jornada: " << workday << "\\n"
       << "Num personal Admin: " << num_administrative_staff << "\\n"
       << "Num Empleados: " << num_employees << "\\n"
       << "Relacion horas-hombre: " << get_relationship_manhours() << "\\n"
       << "Precio: " << price << "\\n"
       << "Otros costos: " << other_cost;

  return sstr.str();
}

Good_Type Product::get_type() const
{
  return PRODUCT;
}


