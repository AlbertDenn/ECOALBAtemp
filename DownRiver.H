# ifndef DownRiver_H
# define DownRiver_H

#include <stdio.h>
#include <iostream>
#include <dbQuery.H>
#include <string>
#include <fstream>
#include <locale>
#include <eunits.H>
#include "bloomFilter.H"




class DownRiver
{

	
	DBConnection connection;
	DBQuery *q = new DBQuery(connection);
	std::string query;

	
	bloom_filter filter;
	
	DynArray<Productos> produccionProductos;
	DynArray<SubUnidadE> subUEconomicas;


	public:

	DownRiver();

	~DownRiver(){}

	DownRiver(std::string);

	DynArray<Proveedores> proveedor(std::string,std::string, std::string);
	void produccion_producto(SubUnidadE, std::string,std::string);
	DynArray<Insumos> produccion_insumo(std::string);
	DynArray<Productos> search(std::string, DynArray<Productos>,bloom_filter);
	UnidadEconomica unidades_economicas(std::string);
	SubUnidadE subUnidadesEconomicas(std::string);
	DynArray<std::string> extract_codes(DynArray<Productos>);

};


DownRiver::DownRiver() 
{
	query=" ";
}

DownRiver::DownRiver(const std::string name)
{
	connection.open(name.c_str());
}


DynArray<std::string> DownRiver::extract_codes(DynArray<Productos> sofar)
{
	DynArray<std::string> codes;

	for(DynArray<Productos>::Iterator it(sofar);it.has_curr();it.next())
		{
			if(!filter.contains(it.get_curr().cod_aran))
			{
			 codes.append(it.get_curr().cod_aran);
			 filter.insert(it.get_curr().cod_aran);
			 std::cout<<std::endl<<it.get_curr().cod_aran<<std::endl;
			}


		}
return codes;

}



DynArray<Productos> DownRiver::search(std::string anho,DynArray<Productos> sofar, bloom_filter filtro)
{
	filter=filtro;

	DBQuery f=*q;

	DynArray<std::string>codigo=extract_codes(sofar);

for(DynArray<std::string>::Iterator it0(codigo);it0.has_curr();it0.next())
{
	f.clear();
	query=(std::string)"SELECT DISTINCT unidadecon_subunidad_economica.ue_rif FROM unidadecon_subunidad_economica, produccion_insumo WHERE produccion_insumo.planta_id_id=unidadecon_subunidad_economica.id AND produccion_insumo.codigo_aran_id='"+it0.get_curr()+"'";
	f.exec(query);
	std::string val;
	SubUnidadE s;
	

		while(f.hasResult() && f.next())
		{
			val.append(f.getValue(0));
			s=subUnidadesEconomicas(f.getValue(0));
			produccion_producto(s,anho,it0.get_curr());		
		}

		for(DynArray<Productos>::Iterator it(produccionProductos);it.has_curr();it.next())
		{
			it.get_curr().insumos=produccion_insumo(it.get_curr().id);
            		it.get_curr().subue.proveedores=proveedor(it.get_curr().subue.id,anho,it0.get_curr());
		}	
}  



return produccionProductos;
	

}

UnidadEconomica DownRiver::unidades_economicas(std::string rif)
{
	DBQuery f=*q;
   	std::size_t found;
	std::string nombre, razon,query;
	UnidadEconomica UE;
	
	f.clear();
	query=(std::string)"SELECT rif, nombre,razon_social FROM unidadecon_unidad_economica WHERE rif="+"\'"+rif+"\'";
	
	f.exec(query);
	f.reset();

	while(f.hasResult() && f.next())
	{
	 rif=f.getValue(0);
	 nombre=f.getValue(1);
	 razon=f.getValue(2);

         
	found=nombre.find("\"");

	while(found!=std::string::npos)
	{

	 nombre.erase(found,1);
	 found=nombre.find("\"");
	
	}
		UE.rif=rif;
		UE.nombre=nombre;	

	}
	f.clear();

	return UE;

	
}


SubUnidadE DownRiver::subUnidadesEconomicas(std::string search)
{	DBQuery f=*q;
	std::size_t found;
	std::string capacidad, nombre, id, entidad;
	SubUnidadE su;
	f.clear();
	query=(std::string)"SELECT unidadecon_subunidad_economica.id, unidadecon_subunidad_economica.nombre, unidadecon_subunidad_economica.capacidad_utilizada, comun_entidad.nombre FROM  unidadecon_subunidad_economica, comun_parroquia,comun_municipio,comun_entidad WHERE unidadecon_subunidad_economica.parroquia_codigo = comun_parroquia.codigo AND comun_parroquia.municipio_codigo=comun_municipio.codigo AND comun_municipio.entidad_codigo = comun_entidad.codigo AND unidadecon_subunidad_economica.tipo_subunidad ='Pl' AND unidadecon_subunidad_economica.ue_rif="+"\'"+search+"\'";
	f.exec(query);

	f.next();
		
		 id=f.getValue(0);
          	 nombre=f.getValue(1);
		 capacidad=f.getValue(2);
		 entidad=f.getValue(3);
        
 		 found=nombre.find("\"");

			while(found!=std::string::npos)
			{
			 nombre.erase(found,1);
			 found=nombre.find("\"");
			}
	
		
		su.id=id;
		su.nombre=nombre;
		su.capacidadu=capacidad;
		su.estado=entidad;
		su.ue= unidades_economicas(search);
return su;

		
}

DynArray<Insumos> DownRiver::produccion_insumo(std::string producto)
{
	DBQuery f=*q;
	std::size_t found;
	std::string cantidad, nombre, id,anio,codigo,marca;
	Insumos p;

	DynArray<Insumos> insumoslist;	

	f.clear();

	query=(std::string)"SELECT DISTINCT produccion_insumo.id, produccion_insumo.nombre, produccion_insumo.codigo_aran_id, produccion_insumo.marca, produccion_consumo_precios_r.cant_comp_imp, produccion_consumo_precios_r.anho FROM produccion_insumo, produccion_consumo_precios_r, produccion_producto_t_insumo, cmproveedores_proveedorinsumo, cmproveedores_proveedor WHERE produccion_insumo.id=produccion_consumo_precios_r.insumo_id_id AND produccion_insumo.id=produccion_producto_t_insumo.insumo_id_id AND produccion_producto_t_insumo.producto_id_id='"+producto+"' AND produccion_insumo.id=cmproveedores_proveedorinsumo.insumo_id_id AND cmproveedores_proveedor.id= cmproveedores_proveedorinsumo.proveedor_id_id AND cmproveedores_proveedor.pais_origen_id<>23";
	f.exec(query);
	

		while(f.hasResult() && f.next())
		{

			
		 anio=f.getValue(5);
                 anio=anio.substr(0,4);
		 id=f.getValue(0);
          	 nombre=f.getValue(1);
		 codigo=f.getValue(2);
		 marca=f.getValue(3);
		 cantidad=f.getValue(4);

	
 		 found=nombre.find("\"");

			while(found!=std::string::npos)
			{
	
			 nombre.erase(found,1);
			 found=nombre.find("\"");
		
			}
	
		p.id=id;
		p.nombre=nombre;
		p.cod_aran=codigo;
		p.cantidad=cantidad;
		p.marca=marca;
		p.anho=anio;
		insumoslist.append(p);
	
		}

	  return insumoslist;
 
}

void DownRiver::produccion_producto(SubUnidadE planta, std::string anho, std::string producto )
{
	DBQuery f=*q;
	std::size_t found;
	std::string cantidad, nombre, id,anio,cantproducido,codigo,marca;
	Productos p;

	f.clear();
	query=(std::string)"SELECT produccion_producto.id, produccion_producto.nombre, produccion_producto.codigo_aran_id, produccion_producto.marca, produccion_produccion_precios_r.cant_producido, produccion_produccion_precios_r.anho FROM produccion_producto, produccion_produccion_precios_r, unidadecon_subunidad_economica, produccion_insumo WHERE produccion_producto.id = produccion_produccion_precios_r.producto_id_id AND produccion_producto.planta_id_id  ='"+planta.id+"' AND  produccion_producto.planta_id_id=unidadecon_subunidad_economica.id AND produccion_insumo.planta_id_id=unidadecon_subunidad_economica.id AND produccion_insumo.codigo_aran_id='"+producto+"'";

	f.exec(query);

		while(f.hasResult() && f.next())
		{

                 anio=f.getValue(5);
		 anio=anio.substr(0,4);

		 id=f.getValue(0);
          	 nombre=f.getValue(1);
		 codigo=f.getValue(2);
		 marca=f.getValue(3);
		 cantidad=f.getValue(4);
        

 		 found=nombre.find("\"");

			while(found!=std::string::npos)
			{
	
			 nombre.erase(found,1);
			 found=nombre.find("\"");
		
			}


			if(anho==anio)
			{		
			p.id=id;
			p.nombre=nombre;
			p.cod_aran=codigo;
			p.cantidad=cantidad;
			p.marca=marca;
			p.anho=anio;
			p.subue=planta;

			if(!filter.contains(p.id))
			{
			produccionProductos.append(p);
			filter.insert(p.id);
			}
			

			
			}
	
		
		}

	
	
}


DynArray<Proveedores> DownRiver::proveedor(std::string search, std::string anho, std::string producto)
{
	DBQuery f=*q;
	std::size_t found;
	std::string id, nombre, rif, paisProcedencia, paisOrigen,anio;
	Proveedores p;
	DynArray<Proveedores> provlist;

	f.clear();
//	query=(std::string)"SELECT produccion_insumo.id, cmproveedores_proveedor.nombre, cmproveedores_proveedor.rif, comun_pais.nombre, comun_pais.nombre, cmproveedores_proveedorinsumo.anho_r FROM cmproveedores_proveedor, comun_pais, cmproveedores_proveedorinsumo, unidadecon_subunidad_economica, produccion_insumo,produccion_producto WHERE cmproveedores_proveedor.pais_origen_id=comun_pais.id AND cmproveedores_proveedor.pais_procedencia_id = comun_pais.id AND cmproveedores_proveedor.pais_origen_id <>23 AND produccion_insumo.id= cmproveedores_proveedorinsumo.insumo_id_id AND cmproveedores_proveedorinsumo.proveedor_id_id=cmproveedores_proveedor.id AND unidadecon_subunidad_economica.id=cmproveedores_proveedor.planta_id_id AND unidadecon_subunidad_economica.id ="+"\'"+search+"\' AND produccion_producto.planta_id_id=unidadecon_subunidad_economica.id AND produccion_producto.codigo_aran_id='"+producto+"'"; // SOLO INTERNACIONALES
	query=(std::string)"SELECT produccion_insumo.id, cmproveedores_proveedor.nombre, cmproveedores_proveedor.rif, comun_pais.nombre, comun_pais.nombre, cmproveedores_proveedorinsumo.anho_r FROM cmproveedores_proveedor, comun_pais, cmproveedores_proveedorinsumo, unidadecon_subunidad_economica, produccion_insumo,produccion_producto WHERE cmproveedores_proveedor.pais_origen_id=comun_pais.id AND cmproveedores_proveedor.pais_procedencia_id = comun_pais.id AND produccion_insumo.id= cmproveedores_proveedorinsumo.insumo_id_id AND cmproveedores_proveedorinsumo.proveedor_id_id=cmproveedores_proveedor.id AND unidadecon_subunidad_economica.id=cmproveedores_proveedor.planta_id_id AND unidadecon_subunidad_economica.id ="+"\'"+search+"\' AND produccion_producto.planta_id_id=unidadecon_subunidad_economica.id AND produccion_producto.codigo_aran_id='"+producto+"'"; //CON NACIONALES
	f.exec(query);
	
	while(f.hasResult() && f.next())
	{
			
		 anio=f.getValue(5);
		 anio=anio.substr(0,4);
		 id=f.getValue(0);
          	 nombre=f.getValue(1);
		 rif=f.getValue(2);
		 paisOrigen=f.getValue(3);
		 paisProcedencia=f.getValue(4);
        
 		 found=nombre.find("\"");

			while(found!=std::string::npos)
			{
	
			 nombre.erase(found,1);
			 found=nombre.find("\"");
		
			}
	
		if(anio==anho)
		{	
		p.insumoID=id;
		p.nombre=nombre;
		p.rif=rif;
		p.paisOrigen=paisOrigen;
		p.paisProcedencia=paisProcedencia;

		provlist.append(p);
							
		}
	
	}
	
	return provlist;

	f.clear();
}




#endif
