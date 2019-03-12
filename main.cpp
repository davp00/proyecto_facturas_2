#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <conio.h>

using namespace std;


struct fecha
{
    int dia;
    int mes;
    int anio;

    bool antes( fecha fecha )
    {
        return dia <= fecha.dia || mes < fecha.mes || anio < fecha.anio;
    }

};


struct factura
{
    int numero;
    string cliente;
    fecha f;
    factura *sig;
};


struct informacion
{
    int numero_factura;
    string producto;
    int cantidad;
    int valor_unitario;
    informacion *sig;
};

bool is_numeric( string cad );
void gotoxy( int x, int y );
int menu();
void limpiar_consola();
void imprimir_menu_principal();
void imprimir_menu_nueva_factura();

factura *insertar_factura( factura *lista, int numero, string cliente, fecha f );
factura *buscar_factura( factura *lista, int numero );
factura *buscar_facturas( factura *lista, fecha f );
int      contar_nodos( factura *lista );
bool     fecha_valida( string dia, string mes, string anio);
fecha   aumentar_fecha( fecha actual );

informacion *insertar_informacion( informacion *lista, int numero_factura, string producto, int cantidad, int valor_unitario );
informacion *informacion_factura( informacion *lista, int numero_factura );
informacion *buscar_producto_factura( informacion *lista, int numFactura ,string producto );
informacion *formulario_producto( int x, int y);
int          total_factura( int numero_factura );


void crear_nueva_factura();
void editar_factura();
void facturas_por_dia();
void facturas_entre_fechas();
void ver_factura();


factura *facturas = NULL;
informacion *productos_facturas = NULL;


int main()
{
    bool sw = false;

    do{
        switch( menu() )
        {
            case 1:
                system("CLS");
                    crear_nueva_factura();
            break;
            case 2:
                system("CLS");
                    if( true )
                        editar_factura();
                    else
                        cout << "No hay facuturas para editar";
                getch();
            break;
            case 3:
                system("CLS");
                    if ( facturas != NULL )
                        ver_factura();
                    else
                        cout << "No hay facuturas para mostrar";
                getch();
            break;

            case 4:
                system("CLS");
                    if( facturas != NULL )
                        facturas_por_dia();
                    else
                        cout << "No hay facuturas para mostrar";

                getch();
            break;

            case 5:
                system("CLS");
                    if( facturas != NULL )
                        facturas_entre_fechas();
                    else
                        cout << "No hay facuturas para mostrar";
                getch();
            break;
            case 0:
                sw = true;
            break;
        }
    }while( sw == false );
	return 0;
}


void crear_nueva_factura() {
    imprimir_menu_nueva_factura();

    factura *nueva = new factura();
    nueva->numero = contar_nodos( facturas ) + 1 ;

    informacion *productos = NULL;

    string dia, mes, anio;

    limpiar_consola();
    gotoxy(34, 3); cout << nueva->numero;
    gotoxy(15, 7); getline( cin, nueva->cliente);
    gotoxy(10, 8); cin  >> dia;
    gotoxy(24, 8); cin  >> mes;
    gotoxy(38, 8); cin  >> anio;

    if( nueva->cliente.empty() == true )
    {
        gotoxy(10, 20);cout << "Todos los campos son requeridos";
        return;
    }

    if ( fecha_valida( dia, mes, anio) == false )
    {
        gotoxy(10, 20);cout << "Formato de fecha no valido";
        getch();
        return;
    }

    nueva->f.dia = atoi( dia.c_str() );
    nueva->f.mes = atoi( mes.c_str() );
    nueva->f.anio = atoi( anio.c_str() );

    bool salir = false;
    system("CLS");

    do
    {
        imprimir_menu_nueva_factura();
        gotoxy(34, 3); cout << nueva->numero;
        gotoxy(15, 7); cout << nueva->cliente;
        gotoxy(10, 8); cout << nueva->f.dia;
        gotoxy(24, 8); cout << nueva->f.mes;
        gotoxy(38, 8); cout << nueva->f.anio;


        gotoxy(70, 2); cout << "Acciones";
        gotoxy(70, 4); cout << "1. Agregar Producto";
        gotoxy(70, 5); cout << "2. Facturar";
        gotoxy(70, 6); cout << "3. Cancelar factura";

        informacion *i = productos;
        int y = 14;
        int total_producto = 0, total = 0;
        while ( i != NULL ) {
            gotoxy(2,y);cout << "²";
            gotoxy(4,y);cout << i->producto;
            gotoxy(25, y); cout << i->cantidad;
            gotoxy(34, y); cout << i->valor_unitario;
            total_producto = i->cantidad * i->valor_unitario;
            total += total_producto;
            gotoxy(46, y); cout << total_producto;
            gotoxy(59, y); cout << "²";
            y++;
            i = i->sig;
        }
        gotoxy(2, y);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(2,++y);printf("                       Total Factura: %d                       ", total);
        gotoxy(2,++y);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");

        switch( getch() - 48 )
        {
            case 1:
            {
                gotoxy(70, 8);cout <<"__________________________________________";
                gotoxy(70, 9);cout << "Informacion del producto";
                informacion *info = formulario_producto( 70, 10 );

                if( info != NULL) {
                    informacion *producto_encontrado = buscar_producto_factura( productos, nueva->numero, info->producto);
                    if( producto_encontrado == NULL ){
                        productos = insertar_informacion( productos, nueva->numero, info->producto, info->cantidad, info->valor_unitario);
                        gotoxy(70,15);cout << "Producto agregado con exito";
                    }else
                    {
                        gotoxy(70,15);cout << "El producto ya existe en la factura";
                    }
                }

                getch();
                system("CLS");
            }
            break;
            case 2:
            {
                facturas = insertar_factura( facturas, nueva->numero, nueva->cliente, nueva->f);

                if ( productos != NULL )
                {
                    while ( productos != NULL ) {
                        productos_facturas = insertar_informacion( productos_facturas, nueva->numero, productos->producto, productos->cantidad, productos->valor_unitario);
                        productos = productos->sig;
                    }
                    gotoxy(70,15); cout << "Factura realizada con exito";
                    salir = true;
                }else
                {
                    gotoxy(70,15);cout << "Debe existir por lo menos un producto en la factura";
                }
                getch();
                system("CLS");
            }
            break;
            case 3:
                salir = true;
            break;
        }

    }while( salir == false );
}


void editar_factura()
{
    string num;
    gotoxy(10, 2);cout << "Buscar Factura para editar.";
    gotoxy(10, 4); cout << "Numero de la factura: ";
    cin >> num;

    if ( is_numeric( num ) == false ){
        gotoxy(10,6); cout << "El campo debe ser numerico";
        return;
    }


    factura *fact = buscar_factura( facturas, atoi( num.c_str() ) );

    if( fact == NULL )
    {
        gotoxy(10,6); cout << "Factura no encontrada en los registros";
        return;
    }

    system("CLS");

    bool salir = false;

    do {
        imprimir_menu_nueva_factura();
        gotoxy(34, 3); cout << fact->numero;
        gotoxy(15, 7); cout << fact->cliente;
        gotoxy(10, 8); cout << fact->f.dia;
        gotoxy(24, 8); cout << fact->f.mes;
        gotoxy(38, 8); cout << fact->f.anio;


        gotoxy(70, 2); cout << "Acciones";
        gotoxy(70, 4); cout << "1. Editar Producto";
        gotoxy(70, 5); cout << "2. Editar Cliente";
        gotoxy(70, 6); cout << "3. Editar Fecha";
        gotoxy(70, 7); cout << "4. Terminar edicion";

        informacion *i = informacion_factura( productos_facturas, fact->numero);
        int y = 14;
        int total_producto = 0, total = 0;
        while ( i != NULL ) {
            gotoxy(2,y);cout << "²";
            gotoxy(4,y);cout << i->producto;
            gotoxy(25, y); cout << i->cantidad;
            gotoxy(34, y); cout << i->valor_unitario;
            total_producto = i->cantidad * i->valor_unitario;
            total += total_producto;
            gotoxy(46, y); cout << total_producto;
            gotoxy(59, y); cout << "²";
            y++;
            i = i->sig;
        }
        gotoxy(2, y);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(2,++y);printf("                       Total Factura: %d                       ", total);
        gotoxy(2,++y);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");

        switch( getch() - 48 ) {

            case 1:
            {
                string producto;
                limpiar_consola();
                gotoxy(70, 8);cout <<"__________________________________________";
                gotoxy(70, 9);cout << "Buscar producto: ";
                getline( cin, producto );

                informacion *producto_encontrado = buscar_producto_factura( productos_facturas, fact->numero, producto);

                if ( producto_encontrado != NULL ) {
                    informacion *info = formulario_producto(70, 12);
                    if ( info != NULL ) {
                        producto_encontrado->producto = info->producto;
                        producto_encontrado->valor_unitario = info->valor_unitario;
                        producto_encontrado->cantidad = info->cantidad;
                        gotoxy(70,19); cout << "Producto Editado con exito";
                    }else {
                        gotoxy(70,19);cout << "Error al editar el producto";
                    }
                } else {
                    gotoxy(70,19); cout << "Producto no encontrado";
                }

                getch();
                system("CLS");
            }
            break;
            case 2:
            {
                limpiar_consola();
                gotoxy(15, 7); cout << "                 ";
                gotoxy(15, 7); getline(cin, fact->cliente);
            }
            break;

            case 3:
            {
                limpiar_consola();
                string dia, mes, anio;
                gotoxy(10, 8); cout << "   ";
                gotoxy(10, 8); cin >> dia;
                gotoxy(24, 8); cout << "   ";
                gotoxy(24, 8); cin >> mes;
                gotoxy(38, 8); cout << "       ";
                gotoxy(38, 8); cin >> anio;

                if ( is_numeric( dia ) == false || is_numeric( mes ) == false || is_numeric( anio ) == false ){
                    gotoxy(70,15);cout << "La fecha debe ser numerica";
                    break;
                }

                if ( fecha_valida(dia, mes, anio) == true )
                {
                    fact->f.dia = atoi( dia.c_str() );
                    fact->f.mes = atoi( mes.c_str() );
                    fact->f.anio= atoi( anio.c_str() );
                    gotoxy(70,15);cout << "Fecha editada con exito";
                }else
                {
                    gotoxy(70, 15); cout << "El formato de la fecha no es valido";
                }

                getch();
                system("CLS");
            }
            break;

            case 4:
                salir = true;
            break;
        }
    }while( salir == false);

}


void ver_factura()
{
    string num;
    gotoxy(10, 2);cout << "Buscar Factura para ver.";
    gotoxy(10, 4); cout << "Numero de la factura: ";
    cin >> num;

    if ( is_numeric( num ) == false ){
        gotoxy(10,6); cout << "El campo debe ser numerico";
        return;
    }

    factura *fact = buscar_factura( facturas, atoi( num.c_str() ) );

    if( fact == NULL )
    {
        gotoxy(10,6); cout << "Factura no encontrada en los registros";
        return;
    }
    imprimir_menu_nueva_factura();
    gotoxy(34, 3); cout << fact->numero;
    gotoxy(15, 7); cout << fact->cliente;
    gotoxy(10, 8); cout << fact->f.dia;
    gotoxy(24, 8); cout << fact->f.mes;
    gotoxy(38, 8); cout << fact->f.anio;

    informacion *i = informacion_factura( productos_facturas, fact->numero);
    int y = 14;
    int total_producto = 0, total = 0;
    while ( i != NULL ) {
        gotoxy(2,y);cout << "²";
        gotoxy(4,y);cout << i->producto;
        gotoxy(25, y); cout << i->cantidad;
        gotoxy(34, y); cout << i->valor_unitario;
        total_producto = i->cantidad * i->valor_unitario;
        total += total_producto;
        gotoxy(46, y); cout << total_producto;
        gotoxy(59, y); cout << "²";
        y++;
        i = i->sig;
    }
    gotoxy(2, y);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
    gotoxy(2,++y);printf("                       Total Factura: %d                       ", total);
    gotoxy(2,++y);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");

}


void facturas_por_dia()
{
    int total_dia = 0, y = 7;
    fecha f;
    string dia, mes, anio;


    gotoxy(10, 2); cout << "Buscar informe por Fecha.";
    gotoxy(10, 3); cout << "Dia: ";
    cin >> dia;
    gotoxy(10, 4); cout << "Mes: ";
    cin >> mes;
    gotoxy(10, 5); cout << "Anio: ";
    cin >> anio;

    if ( fecha_valida( dia, mes, anio ) == false )
    {
        gotoxy(10,7);cout << "El formato de la fecha no es valido";
        return;
    }

    f.dia = atoi( dia.c_str() );
    f.mes = atoi( mes.c_str() );
    f.anio= atoi( anio.c_str() );

    factura *facts  = buscar_facturas( facturas, f);


    if ( facts != NULL ) {
        gotoxy(10, 2); cout << "Fecha: " << f.dia << "/" << f.mes << "/" << f.anio;
        gotoxy(10, 4); cout << "Cliente";
        gotoxy(40, 4); cout << "Total F.";

        gotoxy(10, 5); cout << "________________________________________________________";

        while ( facts != NULL )
        {
            int total_f = total_factura( facts->numero );
            gotoxy(10, y); cout << facts->cliente;
            gotoxy(40, y); cout << total_f;

            total_dia += total_f;
            y++;
            facts = facts->sig;
        }
        gotoxy(29, ++y);cout << "Total dia: "<< total_dia;
    }else
    {
        gotoxy(10,7); cout << "No existen facturas para este dia";
    }
}


void facturas_entre_fechas()
{
    fecha fecha1;
    fecha fecha2;
    string dia, mes, anio;


    gotoxy(10, 2); cout << "Desde.";
    gotoxy(10, 3); cout << "Dia: ";
    cin >> dia;
    gotoxy(10, 4); cout << "Mes: ";
    cin >> mes;
    gotoxy(10, 5); cout << "Anio: ";
    cin >> anio;

    if( fecha_valida( dia, mes, anio) == false ){
        gotoxy(10,7); cout<< "Formato de fecha \"Desde\" No valido";
        return;
    }

    fecha1.dia = atoi( dia.c_str() );
    fecha1.mes = atoi( mes.c_str() );
    fecha1.anio = atoi( anio.c_str() );

    gotoxy(25, 2); cout << "Hasta.";
    gotoxy(25, 3); cout << "Dia: ";
    cin >> dia;
    gotoxy(25, 4); cout << "Mes: ";
    cin >> mes;
    gotoxy(25, 5); cout << "Anio: ";
    cin >> anio;

    if( fecha_valida( dia, mes, anio) == false ){
        gotoxy(25,7); cout<< "Formato de fecha \"Hasta\" No valido";
        return;
    }

    fecha2.dia = atoi( dia.c_str() );
    fecha2.mes = atoi( mes.c_str() );
    fecha2.anio = atoi( anio.c_str() );


    system("CLS");

    gotoxy(10, 2); cout << "Desde " << fecha1.dia << "/" << fecha1.mes << "/" << fecha1.anio
    << " Hasta " << fecha2.dia << "/" << fecha2.mes << "/" << fecha2.anio;
    gotoxy(10, 4); cout << "Fecha";
    gotoxy(40, 4); cout << "Total Dia.";


    gotoxy(10, 5); cout << "________________________________________________________";

    int y = 7;
    int total_rango = 0;
    while ( fecha1.antes( fecha2) )
    {
        factura *facts  = buscar_facturas( facturas, fecha1);
        if( facts != NULL) {
            gotoxy(10, y); cout << fecha1.dia << "/" << fecha1.mes << "/" << fecha1.anio;

            int total_dia = 0;
            while ( facts != NULL )
            {
                int total_f = total_factura( facts->numero );
                total_dia += total_f;
                facts = facts->sig;
            }
            gotoxy(40,y);cout << total_dia;
            y++;

            total_rango += total_dia;
        }
        fecha1 = aumentar_fecha( fecha1 );
    }
    gotoxy(27, ++y);cout << "Total Rango: "<< total_rango;
}

fecha   aumentar_fecha( fecha actual )
{
    int max_dias =
    ( actual.mes == 2 )? 28 : (actual.mes == 4 || actual.mes == 6 || actual.mes == 9 || actual.mes == 11) ? 30 : 31;

    if ( actual.dia < max_dias )
    {
        actual.dia ++;
    }else
    {
        actual.dia = 1;
        if( actual.mes == 12 )
        {
            actual.mes = 1;
            actual.anio ++;
        }else
        {
            actual.mes ++;
        }
    }

    return actual;
}


factura *insertar_factura( factura *lista, int numero, string cliente, fecha f ){
    factura *fact = new factura();
    fact->numero = numero;
    fact->cliente = cliente;
    fact->f = f;

    if (lista == NULL){
        lista = fact;
        lista->sig = NULL;
    }else {
        factura *nodo_anterior = lista;
        while ( nodo_anterior->sig != NULL ) {
            nodo_anterior = nodo_anterior->sig;
        }
        nodo_anterior->sig = fact;
        fact->sig = NULL;
    }

    return lista;
}


factura *buscar_factura( factura *lista, int numero ){
    factura *i = lista;
    while ( i != NULL ){
        if (i->numero == numero)
            return i;
        i = i->sig;
    }

    return NULL;
}


factura *buscar_facturas( factura *lista, fecha f ){
    factura *i = lista;
    factura *nueva_lista = NULL;
    while(i != NULL) {
        if( i->f.dia == f.dia && i->f.mes == f.mes && i->f.anio == f.anio){
            nueva_lista = insertar_factura( nueva_lista, i->numero, i->cliente, i->f);
        }
        i = i->sig;
    }

    return nueva_lista;
}

int contar_nodos( factura *lista ){
    factura *i = lista;
    int contador = 0;
    while(i != NULL){
        contador = contador + 1;
        i = i->sig;
    }
    return contador;
}

informacion *insertar_informacion( informacion *lista, int numero_factura, string producto, int cantidad, int valor_unitario ){
    informacion *info = new informacion();
    info->numero_factura = numero_factura;
    info->producto = producto;
    info->cantidad = cantidad;
    info->valor_unitario = valor_unitario;

    if (lista==NULL){
        lista = info;
        lista->sig = NULL;
    }else {
        informacion *nodo_anterior = lista;
        while ( nodo_anterior->sig != NULL ){
            nodo_anterior = nodo_anterior->sig;
        }
        nodo_anterior->sig = info;
        info->sig = NULL;
    }
    return lista;
}

bool fecha_valida( string dia, string mes, string anio)
{
    if ( is_numeric( dia ) == false || is_numeric( mes ) == false || is_numeric( anio ) == false )
    {
        return false;
    }

    return dia.size() >= 1 && dia.size() <= 2 && mes.size()>= 1 && mes.size() <= 2 && anio.size() == 4;
}

informacion *informacion_factura( informacion *lista, int numero_factura ) {
    informacion *i = lista;
    informacion *nueva_lista = NULL;

    while(i!=NULL){
        if( i->numero_factura == numero_factura){
            nueva_lista = insertar_informacion( nueva_lista, i->numero_factura, i->producto, i->cantidad, i->valor_unitario );
        }
        i = i->sig;
    }
    return nueva_lista;
}

int total_factura( int numero_factura )
{
    informacion *facts = informacion_factura( productos_facturas, numero_factura);
    int total  = 0;

    while ( facts != NULL )
    {
        total += facts->cantidad * facts->valor_unitario;
        facts = facts->sig;
    }

    return total;
}

informacion *buscar_producto_factura( informacion *lista, int numFactura ,string producto ){
    informacion *i = lista;
    while(i!=NULL){
        if(i->numero_factura == numFactura &&  strcmpi(i->producto.c_str(), producto.c_str()) == 0){
                return i;
        }
        i = i->sig;
    }
    return NULL;
}

informacion *formulario_producto( int x, int y) {

    informacion *info = new informacion();
    string cantidad, valor_unit;

    limpiar_consola();
    gotoxy(x, ++y);cout << "Producto: ";
    getline( cin ,info->producto );
    gotoxy(x, ++y);cout << "Cantidad: ";
    cin >> cantidad;

    if ( is_numeric( cantidad )== false)
    {
        gotoxy(x, ++y); cout << "La cantidad debe ser numerica";
        return NULL;
    }
    gotoxy(x, ++y); cout << "Valor unitario: ";
    cin >> valor_unit;
    if ( is_numeric( valor_unit )== false)
    {
        gotoxy(x, ++y); cout << "el valor unitario debe ser numerico";
        return NULL;
    }

    info->cantidad = atoi( cantidad.c_str() );
    info->valor_unitario = atoi( valor_unit.c_str() );

    return info;
}

bool is_numeric( string cad ){
    char numbers[] = { '0','1','2','3','4','5','6','7','9'};
    bool sw = false;
    for( int a = 0 ; a < cad.size() ; a++ ){
        for( int b = 0 ; b < 10 ; b++ ){
            if ( cad[a] == numbers[b] )
                {
                    sw = true;
                }
        }

        if ( sw == false )
        {
            return false;
        }
        sw = false;
    }
    return true;
}




void gotoxy(int x , int y)
{
    HANDLE manipulador;
    COORD Coordenada;

    manipulador = GetStdHandle(STD_OUTPUT_HANDLE);
    Coordenada.X = x;
    Coordenada.Y = y;

    SetConsoleCursorPosition(manipulador,Coordenada);

}


int menu(){
	int tecla;
	bool error=true;

	int intentos=0;
    imprimir_menu_principal();
	do{
	tecla=getch();

		if(tecla>=48 && tecla<=54 || tecla==27){
		error=false;
		return tecla - 48;
		}else{
			intentos++;
			cout<<("Porfavor seleccione un numero del menu\n");
			if(intentos == 4){
					imprimir_menu_principal();
                    intentos=0;
			}
		}

	}while(error);
}


void imprimir_menu_principal() {
        system("CLS");
        gotoxy(30,2);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(30,3);cout << ("²            Menu Principal               ²");
        gotoxy(30,4);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(30,5);cout << ("²               Facturas                  ²");
        gotoxy(30,6);cout << ("²                                         ²");
        gotoxy(30,7);cout << ("²    1-> Nueva Factura                    ²");
        gotoxy(30,8);cout << ("²    2-> Editar Factura                   ²");
        gotoxy(30,9);cout << ("²    3-> Mostrar Factura                  ²");
        gotoxy(30,10);cout << ("²    4-> Informe dado una fecha           ²");
        gotoxy(30,11);cout << ("²    5-> Informe dado un rango de fecha   ²");
        gotoxy(30,12);cout << ("²                                         ²");
        gotoxy(30,13);cout << ("²                                         ²");
        gotoxy(30,14);cout << ("²             0-> Salir                   ²");
        gotoxy(30,15);cout << ("²                                         ²");
        gotoxy(30,16);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
}

void imprimir_menu_nueva_factura(){
        gotoxy(2,2);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(2,3);cout << ("²                    Factura #                           ²");
        gotoxy(2,4);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(2,5);cout << ("²                INFORMACION FACTURA                     ²");
        gotoxy(2,6);cout << ("²                                                        ²");
        gotoxy(2,7);cout << ("²   Cliente:                                             ²");
        gotoxy(2,8);cout << ("²   Dia:          Mes:        Anio:                      ²");
        gotoxy(2,9);cout << ("²                                                        ²");
        gotoxy(2,10);cout << ("²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
        gotoxy(2,11);cout << ("²                                                        ²");
        gotoxy(2,12);cout << ("²  Producto            Cant    Val Unit      Total       ²");
        gotoxy(2,13);cout << ("²                                                        ²");
}

void limpiar_consola()
{
    cin.clear();
    fflush(stdin);
}
