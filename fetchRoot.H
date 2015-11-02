# ifndef FETCHROOt_H
# define FETCHROOt_H

#include <stdio.h>
#include <iostream>
#include <dbQuery.H>
#include <string>
#include <fstream>
#include <locale>
#include <eunits.H>
#include "bloomFilter.H"



class FetchRoot{

	
	private:

	DBConnection connection;
	std::string query;

	bloom_parameters parameters;
	DynDlist <SubUnidadE> subUEconomicas;
	DynDlist <Productos> produccionProductos;
	DynDlist<UnidadEconomica> UEc;
	bloom_filter *filter;
		

	public:

	FetchRoot();

	~FetchRoot(){}

	FetchRoot(std::string);

	DynDlist<UnidadEconomica> searchByCode(std::string,std::string);
	DynDlist<UnidadEconomica> searchByRif(std::string,std::string);
	UnidadEconomica unidades_economicas(std::string);
	SubUnidadE subUnidadesEconomicas(std::string);
	DynDlist<Productos> produccion_producto(SubUnidadE, std::string,std::string,int);
	DynDlist<Productos> produccion_producto(SubUnidadE, std::string);
	DynDlist<Insumos> produccion_insumo(std::string);
	DynDlist <Proveedor> proveedor(std::string, std::string);
	DynDlist <std::string> extract_codes(DynDlist<UnidadEconomica>);
	DynDlist <std::string> extract_codes_insumos(DynDlist<UnidadEconomica>);
	DynDlist <UnidadEconomica> searchDown(DynDlist<UnidadEconomica>,std::string);
	DynDlist <UnidadEconomica> searchUp(DynDlist<UnidadEconomica>,std::string);
		
	};


FetchRoot::FetchRoot() {

	query=" ";
 	parameters.projected_element_count = 1000;
	parameters.false_positive_probability = 0.0001; // 1 in 10000
	parameters.compute_optimal_parameters(); 
	filter=new bloom_filter(parameters);

	
}

FetchRoot::FetchRoot(const std::string name)
{
	connection.open(name.c_str());
 	parameters.projected_element_count = 1000;
	parameters.false_positive_probability = 0.0001; // 1 in 10000
	parameters.compute_optimal_parameters(); 
	filter=new bloom_filter(parameters);

}



DynDlist<std::string> FetchRoot::extract_codes_insumos(DynDlist<UnidadEconomica> sofar)
{
	DynDlist<std::string> codes;

	for(DynDlist<UnidadEconomica>::Iterator it(sofar);it.has_curr();it.next())
		{ filter->insert(it.get_curr().rif);
		for(DynDlist<Productos>::Iterator it0(it.get_curr().productos);it0.has_curr();it0.next())
			for(DynDlist<Insumos>::Iterator it1(it0.get_curr().insumos);it1.has_curr();it1.next())
			{
			std::string code=std::get<0>(it1.get_curr()).cod_aran;
			if(!filter->contains(code))
			codes.append(code);
		

			}
			
			 
		}
return codes;

}


DynDlist<std::string> FetchRoot::extract_codes(DynDlist<UnidadEconomica> sofar)
{
	DynDlist<std::string> codes;

	for(DynDlist<UnidadEconomica>::Iterator it(sofar);it.has_curr();it.next())
		{ 	filter->insert(it.get_curr().rif);
		for(DynDlist<Productos>::Iterator it0(it.get_curr().productos);it0.has_curr();it0.next())

			if(!filter->contains(it0.get_curr().cod_aran))
			 codes.append(it0.get_curr().cod_aran);
			 
		}
return codes;

}


DynDlist<UnidadEconomica> FetchRoot::searchDown(DynDlist<UnidadEconomica> list, std::string anho)
{
	DBQuery f(connection);
	DynDlist<UnidadEconomica> result;
	DynDlist<std::string>codes=extract_codes(list);
	
	UEc.empty();

	for(DynDlist<std::string>::Iterator it(codes);it.has_curr();it.next())
	{
	query=(std::string)"SELECT DISTINCT unidadecon_subunidad_economica.ue_rif FROM unidadecon_subunidad_economica, produccion_insumo WHERE produccion_insumo.planta_id_id=unidadecon_subunidad_economica.id AND produccion_insumo.codigo_aran_id='"+it.get_curr()+"'";
	f.exec(query);
	std::string val;
	SubUnidadE s;
	UnidadEconomica ue;

			while(f.hasResult() && f.next() && !filter->contains(f.getValue(0)))
			{
			val=f.getValue(0);
			s=subUnidadesEconomicas(val);
			ue=unidades_economicas(val);
			ue.planta=s;
			ue.productos=produccion_producto(s,anho,it.get_curr(),1);
			
			std::string id;
			DynDlist<Insumos> ins;
			DynDlist<Proveedor> provs;
			
				for(DynDlist<Productos>::Iterator it0(ue.productos);it0.has_curr();it0.next())
				{
					if(id!=s.id)
					{
					it0.get_curr().insumos=produccion_insumo(it0.get_curr().id);
					ins=it0.get_curr().insumos;
					id=s.id;
					}
					else
					it0.get_curr().insumos=ins;
				}
		
			UEc.append(ue);
			}
		
	}

return UEc;

}


DynDlist<UnidadEconomica> FetchRoot::searchUp(DynDlist<UnidadEconomica> list, std::string anho)
{
	UEc.empty();
	DBQuery f(connection);
	DynDlist<UnidadEconomica> result;
	DynDlist<std::string>codes=extract_codes_insumos(list);
	
	UEc.empty();

	for(DynDlist<std::string>::Iterator it(codes);it.has_curr();it.next())
	{
	query=(std::string)"SELECT DISTINCT unidadecon_subunidad_economica.ue_rif FROM unidadecon_subunidad_economica, produccion_producto WHERE produccion_producto.planta_id_id=unidadecon_subunidad_economica.id AND produccion_producto.codigo_aran_id='"+it.get_curr()+"'";
	f.exec(query);
	std::string val;
	SubUnidadE s;
	UnidadEconomica ue;

			while(f.hasResult() && f.next() && !filter->contains(f.getValue(0)))
			{
			val=f.getValue(0);
			s=subUnidadesEconomicas(val);
			ue=unidades_economicas(val);
			ue.planta=s;
			ue.productos=produccion_producto(s,anho,it.get_curr(),2);
			
			std::string id;
			DynDlist<Insumos> ins;
			DynDlist<Proveedor> provs;
			
				for(DynDlist<Productos>::Iterator it0(ue.productos);it0.has_curr();it0.next())
				{
					if(id!=s.id)
					{
					it0.get_curr().insumos=produccion_insumo(it0.get_curr().id);
					ins=it0.get_curr().insumos;
					id=s.id;
					}
					else
					it0.get_curr().insumos=ins;
				}
		
			UEc.append(ue);
			}
		
	}

return UEc;

}
 

DynDlist<UnidadEconomica> FetchRoot::searchByCode(std::string rif, std::string anho)
{
		UEc.empty();
	DBQuery f(connection);
	query=(std::string)"SELECT DISTINCT unidadecon_subunidad_economica.ue_rif FROM unidadecon_subunidad_economica, produccion_producto WHERE unidadecon_subunidad_economica.tipo_subunidad='Pl' AND produccion_producto.codigo_aran_id='"+rif+"'AND produccion_producto.planta_id_id=unidadecon_subunidad_economica.id";
	f.exec(query);
	std::string val;
	SubUnidadE s;
	UnidadEconomica ue;

			while(f.hasResult() && f.next())
			{
			val=f.getValue(0);
			s=subUnidadesEconomicas(val);
			ue=unidades_economicas(val);
			ue.planta=s;
			ue.productos=produccion_producto(s,anho,rif,0);
			

			std::string id;
			DynDlist<Insumos> ins;
			DynDlist<Proveedor> provs;
			
				for(DynDlist<Productos>::Iterator it(ue.productos);it.has_curr();it.next())
				{
					if(id!=s.id)
					{
					it.get_curr().insumos=produccion_insumo(it.get_curr().id);
					ins=it.get_curr().insumos;
					id=s.id;
					}
					else
					it.get_curr().insumos=ins;
				}
		
			UEc.append(ue);
			}


return UEc;
}


DynDlist<UnidadEconomica> FetchRoot::searchByRif(std::string rif, std::string anho)
{

	UEc.empty();
		SubUnidadE s;
		UnidadEconomica ue;

		s=subUnidadesEconomicas(rif);	
		ue=unidades_economicas(rif);
		ue.productos=produccion_producto(s,anho);		
		
		std::string id;
		DynDlist<Insumos> ins;
		DynDlist<Proveedor> provs;

				for(DynDlist<Productos>::Iterator it1(ue.productos);it1.has_curr();it1.next())
				{
					if(id!=s.id)
					{
					it1.get_curr().insumos=produccion_insumo(it1.get_curr().id);
					ins=it1.get_curr().insumos;
					id=s.id;
					}
					else
					it1.get_curr().insumos=ins;
				}
		
			UEc.append(ue);

return UEc;
}



UnidadEconomica FetchRoot::unidades_economicas(std::string rif)
{
	DBQuery f(connection);
   	std::size_t found;
	std::string nombre, razon,query;
	UnidadEconomica UE;

	query=(std::string)"SELECT rif, nombre,razon_social FROM unidadecon_unidad_economica WHERE rif="+"\'"+rif+"\'";
	f.exec(query);


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
	UE.razon=razon;	

	}

return UE;

	
}


SubUnidadE FetchRoot::subUnidadesEconomicas(std::string search)
{
	DBQuery f(connection);

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

return su;
}

DynDlist<Insumos> FetchRoot::produccion_insumo(std::string producto)
{

	DBQuery f(connection);
	std::size_t found;
	std::string cantidad, nombre, id,anio,codigo,marca;
	Imported impo;
	DynDlist<Proveedor> prov;
	Insumos in;

	DynDlist<Insumos> result;


	f.clear();
	query=(std::string)"SELECT DISTINCT produccion_insumo.id, produccion_insumo.nombre, produccion_insumo.codigo_aran_id, produccion_insumo.marca, produccion_consumo_precios_r.cant_comp_imp, produccion_consumo_precios_r.anho FROM produccion_insumo, produccion_consumo_precios_r, produccion_producto_t_insumo, cmproveedores_proveedorinsumo, cmproveedores_proveedor WHERE produccion_insumo.id=produccion_consumo_precios_r.insumo_id_id AND produccion_insumo.id=produccion_producto_t_insumo.insumo_id_id AND produccion_producto_t_insumo.producto_id_id='"+producto+"' AND produccion_insumo.id=cmproveedores_proveedorinsumo.insumo_id_id AND cmproveedores_proveedor.id= cmproveedores_proveedorinsumo.proveedor_id_id AND cmproveedores_proveedor.pais_origen_id <> 23";
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
	
		impo.id=id;
		impo.nombre=nombre;
		impo.cod_aran=codigo;
		impo.cantidad=cantidad;
		impo.marca=marca;
		impo.anho=anio;
		
		if(!filter->contains(impo.nombre))
		{
		prov=proveedor(impo.id,anio);
		std::get<0>(in)=impo;
		std::get<1>(in)=prov;
		result.append(in);		
		filter->insert(impo.nombre);
		}


		}

 return result;
 
}


DynDlist<Productos> FetchRoot::produccion_producto(SubUnidadE planta, std::string anho, std::string producto, int y )
{

	DBQuery f(connection);
	std::size_t found;
	std::string cantidad, nombre, id,anio,cantproducido,codigo,marca;
	Productos p;
	DynDlist<Productos> prods;

	f.clear();
	if(y==0)
	query=(std::string)"SELECT produccion_producto.id, produccion_producto.nombre, produccion_producto.codigo_aran_id, produccion_producto.marca, produccion_produccion_precios_r.cant_producido, produccion_produccion_precios_r.anho FROM produccion_producto, produccion_produccion_precios_r WHERE produccion_producto.id = produccion_produccion_precios_r.producto_id_id AND produccion_producto.planta_id_id  ='"+planta.id+"' AND produccion_producto.codigo_aran_id='"+producto+"'";
	if(y==1)
	query=(std::string)"SELECT produccion_producto.id, produccion_producto.nombre, produccion_producto.codigo_aran_id, produccion_producto.marca, produccion_produccion_precios_r.cant_producido, produccion_produccion_precios_r.anho FROM produccion_producto, produccion_produccion_precios_r, unidadecon_subunidad_economica, produccion_insumo WHERE produccion_producto.id = produccion_produccion_precios_r.producto_id_id AND unidadecon_subunidad_economica.id  ='"+planta.id+"' AND  produccion_producto.planta_id_id=unidadecon_subunidad_economica.id AND produccion_insumo.planta_id_id=unidadecon_subunidad_economica.id AND produccion_insumo.codigo_aran_id='"+producto+"'";
	if(y==2)
	//query=(std::string)"SELECT produccion_producto.id, produccion_producto.nombre, produccion_producto.codigo_aran_id, produccion_producto.marca, produccion_produccion_precios_r.cant_producido, produccion_produccion_precios_r.anho FROM produccion_producto, produccion_produccion_precios_r, unidadecon_subunidad_economica, produccion_insumo WHERE produccion_producto.id = produccion_produccion_precios_r.producto_id_id AND unidadecon_subunidad_economica.id  ='"+planta.id+"' AND  produccion_producto.planta_id_id=unidadecon_subunidad_economica.id AND produccion_insumo.planta_id_id=unidadecon_subunidad_economica.id AND produccion_producto.codigo_aran_id='"+producto+"'";
	query=(std::string)"SELECT produccion_producto.id, produccion_producto.nombre, produccion_producto.codigo_aran_id, produccion_producto.marca, produccion_produccion_precios_r.cant_producido, produccion_produccion_precios_r.anho FROM produccion_producto, produccion_produccion_precios_r, unidadecon_subunidad_economica, produccion_insumo WHERE produccion_producto.id = produccion_produccion_precios_r.producto_id_id AND  produccion_producto.planta_id_id=unidadecon_subunidad_economica.id AND produccion_insumo.planta_id_id=unidadecon_subunidad_economica.id AND produccion_producto.codigo_aran_id=produccion_insumo.codigo_aran_id AND produccion_producto.codigo_aran_id='"+producto+"'";
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
				if(prods.is_empty() || !filter->contains(p.nombre))
				{
				prods.append(p);
				filter->insert(p.nombre);			
				}
				
			}
	
		
		}

return prods;

}

DynDlist<Productos> FetchRoot::produccion_producto(SubUnidadE planta, std::string anho)
{

	DBQuery f(connection);
	std::size_t found;
	std::string cantidad, nombre, id,anio,cantproducido,codigo,marca;
	Productos p;
	DynDlist<Productos> prods;

	f.clear();
	query=(std::string)"SELECT produccion_producto.id, produccion_producto.nombre, produccion_producto.codigo_aran_id, produccion_producto.marca, produccion_produccion_precios_r.cant_producido, produccion_produccion_precios_r.anho FROM produccion_producto, produccion_produccion_precios_r WHERE produccion_producto.id = produccion_produccion_precios_r.producto_id_id AND produccion_producto.planta_id_id  ='"+planta.id+"'";
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
				if(prods.is_empty() || !filter->contains(p.id))
				{
				prods.append(p);
				filter->insert(p.id);			
				}
				
			}
	
		
		}

return prods;

}


DynDlist<Proveedor> FetchRoot::proveedor(std::string producto, std::string anho)
{

	DBQuery f(connection);
	std::size_t found;
	std::string id, nombre, rif, paisProcedencia, paisOrigen,anio;
	Proveedor p;
	DynDlist<Proveedor> provlist;

	f.clear();
	query=(std::string)"SELECT DISTINCT cmproveedores_proveedor.nombre, cmproveedores_proveedor.rif, comun_pais.nombre, comun_pais.nombre, cmproveedores_proveedorinsumo.anho_r FROM cmproveedores_proveedor, comun_pais, cmproveedores_proveedorinsumo WHERE cmproveedores_proveedor.pais_origen_id=comun_pais.id AND cmproveedores_proveedor.pais_procedencia_id = comun_pais.id AND cmproveedores_proveedorinsumo.proveedor_id_id=cmproveedores_proveedor.id AND cmproveedores_proveedorinsumo.insumo_id_id='"+producto+"'"; //CON NACIONALES
	f.exec(query);
	
	while(f.hasResult() && f.next())
	{
			
		 anio=f.getValue(4);
		 anio=anio.substr(0,4);
		 nombre=f.getValue(0);
		 rif=f.getValue(1);
		 paisOrigen=f.getValue(2);
		 paisProcedencia=f.getValue(3);
        
 		 found=nombre.find("\"");

			while(found!=std::string::npos)
			{
	
			 nombre.erase(found,1);
			 found=nombre.find("\"");
		
			}
	
		if(anio==anho)
		{	
		p.insumoID=producto;
		p.nombre=nombre;
		p.rif=rif;
		p.paisOrigen=paisOrigen;
		p.paisProcedencia=paisProcedencia;

			if(provlist.is_empty() || !filter->contains(p.nombre))
			{
				provlist.append(p);
				filter->insert(p.nombre);
			}
	
			
		}
	
	}
	
return provlist;

}





#endif



