# include "AIPlayer.h"
# include "Parchis.h"

const double masinf = 9999999999.0, menosinf = -9999999999.0;
const double gana = masinf - 1, pierde = menosinf + 1;
const int num_pieces = 3;
const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 6; // Umbral maximo de profundidad para la poda Alfa_Beta

bool AIPlayer::move(){
    cout << "Realizo un movimiento automatico" << endl;

    color c_piece;
    int id_piece;
    int dice;
    think(c_piece, id_piece, dice);

    cout << "Movimiento elegido: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    actual->movePiece(c_piece, id_piece, dice);
    return true;
}

void AIPlayer::think(color & c_piece, int & id_piece, int & dice) const{
    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha = menosinf, beta = masinf; // Cotas iniciales de la poda AlfaBeta
        
    
    switch(id){
        case 0:
            thinkAleatorio(c_piece, id_piece, dice);
        break;
        case 1:
            thinkAleatorioMasInteligente(c_piece, id_piece, dice);
        break;
        case 2:
            thinkFichaMasAdelantada(c_piece, id_piece, dice);
        break;
        case 3:
            thinkMejorOpcion(c_piece, id_piece, dice);
        break;
        case 4:
            cout<<"estoy pensando"<<endl;
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
            cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;
        break;
        case 5:
            cout<<"pienso, luego existo"<<endl;
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion);
            cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;        
        break;
    }

    /*
    // El siguiente código se proporciona como sugerencia para iniciar la implementación del agente.

    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha = menosinf, beta = masinf; // Cotas iniciales de la poda AlfaBeta
    // Llamada a la función para la poda (los parámetros son solo una sugerencia, se pueden modificar).
    valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
    cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    // ----------------------------------------------------------------- //

    // Si quiero poder manejar varias heurísticas, puedo usar la variable id del agente para usar una u otra.
    switch(id){
        case 0:
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
            break;
        case 1:
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion1);
            break;
        case 2:
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion2);
            break;
    }
    cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    */
}

void AIPlayer::thinkAleatorio(color & c_piece, int & id_piece, int & dice) const{
    // IMPLEMENTACIÓN INICIAL DEL AGENTE
    // Esta implementación realiza un movimiento aleatorio.
    // Se proporciona como ejemplo, pero se debe cambiar por una que realice un movimiento inteligente
    //como lo que se muestran al final de la función.

    // OBJETIVO: Asignar a las variables c_piece, id_piece, dice (pasadas por referencia) los valores,
    //respectivamente, de:
    // - color de ficha a mover
    // - identificador de la ficha que se va a mover
    // - valor del dado con el que se va a mover la ficha.

    // El id de mi jugador actual.
    int player = actual->getCurrentPlayerId();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<tuple<color, int>> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices = actual->getAvailableNormalDices(player);
    // Elijo un dado de forma aleatoria.
    dice = current_dices[rand() % current_dices.size()];

    // Se obtiene el vector de fichas que se pueden mover para el dado elegido
    current_pieces = actual->getAvailablePieces(player, dice);

    // Si tengo fichas para el dado elegido muevo una al azar.
    if (current_pieces.size() > 0)
    {
        int random_id = rand() % current_pieces.size();
        id_piece = get<1>(current_pieces[random_id]); // get<i>(tuple<...>) me devuelve el i-ésimo
        c_piece = get<0>(current_pieces[random_id]);  // elemento de la tupla
    }
    else
    {
        // Si no tengo fichas para el dado elegido, pasa turno (la macro SKIP_TURN me permite no mover).
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
    }


}

void AIPlayer::thinkAleatorioMasInteligente(color &c_piece, int &id_piece, int &dice) const
{
    // El número de mi jugador actual.
    int player = actual->getCurrentPlayerId();
    // Vector que almacenará los dados que se pueden usar para el movimiento.
    vector<int> current_dices_con_especiales;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<tuple<color, int>> current_pieces;
    // Obtengo el vector de dados que puedo usar para el movimiento.
    // En este caso elijo todos, tanto normales como especiales.
    // Importante: puedo acceder a solo dados normales o especiales por separado,
    // o a todos a la vez:
    // - actual->getAvailableNormalDices(player) -> solo dados normales
    // - actual->getAvailableSpecialDices(player) -> solo dados especiales
    // - actual->getAllAvailableDices(player) -> todos los dados
    // Importante 2: los "available" me dan los dados que puedo usar en el turno actual.
    // Por ejemplo, si me tengo que contar 10 o 20 solo me saldrán los dados 10 y 20.
    // Puedo saber qué más dados tengo, aunque no los pueda usar en este turno, con:
    // - actual->getNormalDices(player) -> todos los dados normales
    // - actual->getSpecialDices(player) -> todos los dados especiales
    // - actual->getAllDices(player) -> todos los dados
    current_dices_con_especiales = actual->getAllAvailableDices(player);
    // En vez de elegir un dado al azar, miro primero cuáles tienen fichas que se puedan mover.
    vector<int> current_dices_que_pueden_mover_ficha;
    for (int i = 0; i < current_dices_con_especiales.size(); i++)
    {
        // Se obtiene el vector de fichas que se pueden mover para el dado elegido.
        current_pieces = actual->getAvailablePieces(player, current_dices_con_especiales[i]);
        // Si se pueden mover fichas para el dado actual, lo añado al vector de dados que pueden mover fichas.
        if (current_pieces.size() > 0){
            current_dices_que_pueden_mover_ficha.push_back(current_dices_con_especiales[i]);
        }
    }
        // Si no tengo ninún dado que pueda mover fichas, paso turno con un dado al azar (la macro SKIP_TURN me permite no mover).
    if (current_dices_que_pueden_mover_ficha.size() == 0)
    {
        dice = current_dices_con_especiales[rand() % current_dices_con_especiales.size()];
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
    }
    // En caso contrario, elijo un dado de forma aleatoria de entre los que pueden mover ficha.
    else
    {
        dice = current_dices_que_pueden_mover_ficha[rand() % current_dices_que_pueden_mover_ficha.size()];
        // Se obtiene el vector de fichas que se pueden mover para el dado elegido.
        current_pieces = actual->getAvailablePieces(player, dice);
        // Muevo una ficha al azar de entre las que se pueden mover.
        int random_id = rand() % current_pieces.size();
        id_piece = get<1>(current_pieces[random_id]);
        c_piece = get<0>(current_pieces[random_id]);
    }
}

void AIPlayer::thinkFichaMasAdelantada(color &c_piece, int &id_piece, int &dice) const
{
    // Elijo el dado haciendo lo mismo que el jugador anterior.
    thinkAleatorioMasInteligente(c_piece, id_piece, dice);

    // Tras llamar a esta función, ya tengo en dice el número de dado que quiero usar.
    // Ahora, en vez de mover una ficha al azar, voy a mover (o a aplicar
    // el dado especial a) la que esté más adelantada
    // (equivalentemente, la más cercana a la meta).
    int player = actual->getCurrentPlayerId();
    vector<tuple<color, int>> current_pieces = actual->getAvailablePieces(player, dice);
    int id_ficha_mas_adelantada = -1;
    color col_ficha_mas_adelantada = none;
    int min_distancia_meta = 9999;
    
    //es un simple calculo de un minimo
    for (int i = 0; i < current_pieces.size(); i++)
    {
        // distanceToGoal(color, id) devuelve la distancia a la meta de la ficha [id] del color que le indique.
        color col = get<0>(current_pieces[i]);
        int id = get<1>(current_pieces[i]);
        int distancia_meta = actual->distanceToGoal(col, id);
        
        if (distancia_meta < min_distancia_meta){
            min_distancia_meta = distancia_meta;
            id_ficha_mas_adelantada = id;
            col_ficha_mas_adelantada = col;
        }
    }
    // Si no he encontrado ninguna ficha, paso turno.
    if (id_ficha_mas_adelantada == -1)
    {
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
    }
    // En caso contrario, moveré la ficha más adelantada.
    else
    {
        id_piece = id_ficha_mas_adelantada;
        c_piece = col_ficha_mas_adelantada;
    }
}

void AIPlayer::thinkMejorOpcion(color &c_piece, int &id_piece, int &dice) const{
    // Vamos a mirar todos los posibles movimientos del jugador actual accediendo a los hijos del estado actual.
    // Para ello, vamos a iterar sobre los hijos con la función de Parchis getChildren().
    // Esta función devuelve un objeto de la clase ParchisBros, que es una estructura iterable
    // sobre la que se pueden recorrer todos los hijos del estado sobre el que se llama.
    ParchisBros hijos = actual->getChildren();
    bool me_quedo_con_esta_accion = false;
    // La clase ParchisBros viene con un iterador muy útil y sencillo de usar.
    // Al hacer begin() accedemos al primer hijo de la rama,
    // y cada vez que hagamos ++it saltaremos al siguiente hijo.
    // Comparando con el iterador end() podemos consultar cuándo hemos terminado de visitar los hijos.
    for(ParchisBros::Iterator it = hijos.begin(); it != hijos.end() and !me_quedo_con_esta_accion; ++it){
        Parchis siguiente_hijo = *it; // Accedemos al tablero hijo con el operador de indirección.

        //si como, meto en meta o gano entonces cojo ese movimiento
        if(siguiente_hijo.isEatingMove() or siguiente_hijo.isGoalMove() or (siguiente_hijo.gameOver() and siguiente_hijo.getWinner() == this->jugador) ) {
            me_quedo_con_esta_accion = true;
            c_piece = it.getMovedColor(); // Guardo color de la ficha movida.
            id_piece = it.getMovedPieceId(); // Guardo id de la ficha movida.
            dice = it.getMovedDiceValue(); // Guardo número de dado movido.
        }
    }
    // Si he encontrado una acción que me interesa, la guardo en las variables pasadas por referencia.
    // (Ya lo he hecho antes, cuando les he asignado los valores con el iterador).
    // Si no, muevo la ficha más adelantada como antes.
    if(!me_quedo_con_esta_accion){
        thinkFichaMasAdelantada(c_piece, id_piece, dice);
    }
}

double AIPlayer::Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristic)(const Parchis &, int)) const{

    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    
    //miro los hijos en profundidad
    ParchisBros hijos = actual.getChildren();
    
    //si siguiente_hijo es de profundidad 6 (seria el ultimo q vemos) o es nodo hoja (se acaba el juego)
    if (profundidad==PROFUNDIDAD_ALFABETA or actual.gameOver()){ 
        valor=heuristic(actual, jugador);
        return valor;
    }
    
    //max: llama al alg de busqueda, si saca un 6, come ficha o introduce en meta
    if(actual.getCurrentPlayerId()==jugador) {    
        //cout<<"MAX"<<endl;
        double alpha_anterior;
        
        for(ParchisBros::Iterator it = hijos.begin(); it != hijos.end(); ++it){ //para cada hijo
            Parchis siguiente_hijo = *it; // Accedemos al tablero hijo con el operador de indirección
            //cout<<"itero"<<endl;
            //actualizamos el valor de alpha
            alpha_anterior=alpha;
            alpha=max(alpha, Poda_AlfaBeta(siguiente_hijo, jugador, profundidad+1, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, heuristic));
            //cout<<alpha<<endl;
            valor=alpha;

            if (beta<=alpha){   //se han intercambiado y podemos podar (SALE)
                //valor=beta;
                break;
            }
            if (profundidad==0 and alpha!=alpha_anterior and valor!=menosinf){ //solo consideramos el mov cuando alfa se ha actualizado
                //cout<<"profundidad 0"<<endl;
                c_piece = it.getMovedColor(); // Guardo color de la ficha movida.
                id_piece = it.getMovedPieceId(); // Guardo id de la ficha movida.
                dice = it.getMovedDiceValue(); // Guardo número de dado movido.
            }
            
        }

    }else if (actual.getCurrentPlayerId()!=jugador){
        //cout<<"MIN"<<endl;
        for(ParchisBros::Iterator it = hijos.begin(); it != hijos.end(); ++it){
            Parchis siguiente_hijo = *it; // Accedemos al tablero hijo con el operador de indirección
            beta=min(beta, Poda_AlfaBeta(siguiente_hijo, jugador, profundidad+1, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, heuristic));
            valor=beta;

            if (beta<=alpha){   //se han intercambiado y podemos podar
                //valor=alpha;
                break;
            }
        }   
    }

    return valor;  
}


double AIPlayer::ValoracionTest(const Parchis &estado, int jugador)
{
    // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.


    int ganador = estado.getWinner();
    int oponente = (jugador+1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador++;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_jugador += 5;
                }
            }
        }

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                if (estado.isSafePiece(c, j))
                {
                    // Valoro negativamente que la ficha esté en casilla segura o meta.
                    puntuacion_oponente++;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_oponente += 5;
                }
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}

double AIPlayer::MiValoracion(const Parchis &estado, int jugador)
{
    //Mi heurística añadiendo la distancia a la meta 

    int ganador = estado.getWinner();
    int oponente = (jugador+1) % 2;


    // Colores que juega mi jugador y colores del oponente
    vector<color> my_colors = estado.getPlayerColors(jugador);
    vector<color> op_colors = estado.getPlayerColors(oponente);

    //todos los dados
    vector<int> current_dices_con_especiales;   
    current_dices_con_especiales = estado.getAllAvailableDices(jugador);    

    //solo dados especiales
    vector<int> current_dices_especiales;   
    current_dices_especiales = estado.getAvailableSpecialDices(jugador);

    //solo dados especiales
    vector<int> current_dices_especiales_oponente;   
    current_dices_especiales_oponente = estado.getAvailableSpecialDices(oponente);
    
    // Recorro todas las fichas de mi jugador
    int puntuacion_jugador = 0;
    int puntuacion_un_color=0;
    int puntuacion_otro_color=0;
    int distancia_un_color=0;
    int distancia_otro_color=0;
    bool casi_gano=false;

    // Recorro colores de mi jugador.
    for (int i = 0; i < my_colors.size(); i++)
    {
        color c = my_colors[i];

        //potencio un color 
        /*if (estado.piecesAtGoal(my_colors[0])==1){
            puntuacion_un_color+=50;
        }else if (estado.piecesAtGoal(my_colors[0])==2){
            puntuacion_un_color+=100;
        }

        if (estado.piecesAtGoal(my_colors[1])==1){
            puntuacion_otro_color+=50;
        }else if (estado.piecesAtGoal(my_colors[1])==2){
            puntuacion_otro_color+=75;
        }

        if (puntuacion_un_color<puntuacion_otro_color){
            puntuacion_otro_color*10;
        }else {
            puntuacion_un_color*10;
        }*/

        // Recorro las fichas de ese color.
        for (int j = 0; j < num_pieces; j++)
        {
            // Valoro positivamente que la ficha esté en casilla segura o meta.
            if (estado.isSafePiece(c, j))
            {
                puntuacion_jugador+=10;
            }

            if (estado.getBoard().getPiece(c, j).get_box().type == final_queue )
            {//NUEVO
                puntuacion_jugador+=30;
            }

            /*if (estado.isSafePiece(c, j) and estado.piecesAtGoal(c)==1)
            {
                puntuacion_jugador+=20;
            }if (estado.isSafePiece(c, j) and estado.piecesAtGoal(c)==2)
            {
                puntuacion_jugador+=25;
            }
            else if (!estado.isSafePiece(c, j) and estado.piecesAtGoal(c)==2){
                puntuacion_jugador-=10;
            }*/



            if (estado.getBoard().getPiece(c, j).get_box().type == goal)
            {
                puntuacion_jugador += 150;
            }

            
            if (i==0){
                distancia_un_color +=estado.distanceToGoal(c,estado.getBoard().getPiece(c, j).get_box());
            }if (i==1){
                distancia_otro_color +=estado.distanceToGoal(c,estado.getBoard().getPiece(c, j).get_box());
            }

            /*if (estado.isEatingMove() and (estado.eatenPiece().first!=my_colors[(i+1)%2])){  //si el q me como no es mi otro color
                puntuacion_jugador += 30;
            }else if (estado.isEatingMove()){   //si me como a mi mismo (hay veces que renta?)
                puntuacion_jugador += 5;
            }*/

            /*if (estado.isEatingMove()){
                puntuacion_jugador += 5;
            }*/

            //INTENTO POTENCIAR EL COLOR QUE TIENE MAS EN LA META-> Y ESTOOOOOOOO PPORQUEEEEEE NOOOOOOO MEJOOOOORAAAAAAAAAAAA
            /*if (get<0>(estado.getLastAction())==c and estado.piecesAtGoal(c)==2){
                puntuacion_jugador+=50;
            }else  if (get<0>(estado.getLastAction())==c and estado.piecesAtGoal(c)==1){
                puntuacion_jugador+=2;
            }/*else if (get<0>(estado.getLastAction())!=c and estado.piecesAtGoal(c)==2){
                puntuacion_jugador-=20;
            }*/

            if (estado.piecesAtGoal(c)==1){
                puntuacion_jugador += 30;
            }else if (estado.piecesAtGoal(c)==2){
                puntuacion_jugador += 70;
                casi_gano=true;
            }else if (estado.piecesAtGoal(c)==3){//ESTO LO HE AÑADIDO NUUUEVOO PARA QUE QUIERA METER EN CASA
                puntuacion_jugador += 6000;
            }
            if (estado.piecesAtHome(c)==1){
                puntuacion_jugador -= 20;
            }else if (estado.piecesAtHome(c)==2){
                puntuacion_jugador -= 50;
            }else if (estado.piecesAtHome(c)==3){
                puntuacion_jugador -= 250;
            }
            
            //if (casi_gano){ //quiero potenciar ese color

            //}
            //que haya una trampa en la siguiente casilla (realmente quiero en el final pero no se) 
            /*if ((estado.anyTrap(estado.getBoard().getPiece(c, j).get_box(), estado.nextBox(c,(estado.getBoard().getPiece(c, j).get_box())))).size()>0){
                puntuacion_jugador-=2; 
            }*/

            //OTRA IDEA: hacer barrera-> esto EMPEORA
            /*if (estado.isWall(estado.getBoard().getPiece(c, j).get_box())==c){
                puntuacion_jugador+=5; 
            }*/

            //le restamos la distancia pq quiero q se le de preferencia al que este mas cerca de la meta
            puntuacion_jugador -=estado.distanceToGoal(c,estado.getBoard().getPiece(c, j).get_box());
        }
    }

    //le quiero dar preferencia a ese color
    if (distancia_un_color<distancia_otro_color){
            puntuacion_jugador-=0.33*distancia_un_color;
    }else {
        puntuacion_jugador-=0.33*distancia_otro_color;
    }

    //OTRA IDEA: darle puntos a que se coma tb su propias fichas si ya tiene dos de un mismo color en goal
    

   
    //Añado objetos especiales con puntuacion
    for (int i = 0; i < current_dices_especiales.size(); i++)
    {
        if (current_dices_especiales[i]==banana){  //platano
            puntuacion_jugador += 2;
        }if (current_dices_especiales[i]==mushroom){  //champiñon
            puntuacion_jugador += 7;
        }if (current_dices_especiales[i]==red_shell){  //Caparazón rojo
            puntuacion_jugador += 30;
        }if (current_dices_especiales[i]==blue_shell){  //Caparazón azul
            puntuacion_jugador += 50;
        }if (current_dices_especiales[i]==horn){  //Bocina
            puntuacion_jugador += 30;
        }if (current_dices_especiales[i]==bullet){  //Bala
            puntuacion_jugador += 35;
        }if (current_dices_especiales[i]==shock){  //Rayo
            puntuacion_jugador += 50;
        }if (current_dices_especiales[i]==boo){  //Boo
            puntuacion_jugador += 20;
        }if (current_dices_especiales[i]==star){  //Estrella
            puntuacion_jugador += 45;
        }if (current_dices_especiales[i]==mega_mushroom){  //Megachampiñón
            puntuacion_jugador += 40;
        }
    }


    // Recorro todas las fichas del oponente
    int puntuacion_oponente = 0;
    // Recorro colores del oponente.
    for (int i = 0; i < op_colors.size(); i++)
    {
        color c = op_colors[i];

        // Recorro las fichas de ese color.
        for (int j = 0; j < num_pieces; j++)
        {
            if (estado.isSafePiece(c, j))
            {
                // Valoro negativamente que la ficha esté en casilla segura o meta.
                puntuacion_oponente+=10;
            }
            /*if (estado.isSafePiece(c, j) and estado.piecesAtGoal(c)==2)
            {
                puntuacion_oponente+=25;
            }else if (!estado.isSafePiece(c, j) and estado.piecesAtGoal(c)==2){
                puntuacion_oponente-=10;
            }*/

            if (estado.getBoard().getPiece(c, j).get_box().type == final_queue )
            {
                puntuacion_oponente+=30;
            }


            if (estado.getBoard().getPiece(c, j).get_box().type == goal)
            {
                puntuacion_oponente += 150;
            }


            /*if (estado.isEatingMove() and (estado.eatenPiece().first!=op_colors[(i+1)%2])){  //si el q me como no es mi otro color
                puntuacion_oponente += 50;
            }else if (estado.isEatingMove()){
                puntuacion_oponente += 5;
            }*/
            /*if (estado.isEatingMove()){
                puntuacion_oponente += 5;
            }*/


            if (estado.piecesAtGoal(c)==1){
                puntuacion_oponente += 30;
            }else if (estado.piecesAtGoal(c)==2){
                puntuacion_oponente += 70;
            }if (estado.piecesAtGoal(c)==3){
                puntuacion_oponente += 6000;
            }

            if (estado.piecesAtHome(c)==1){
                puntuacion_oponente -= 20;
            }else if (estado.piecesAtHome(c)==2){
                puntuacion_oponente -= 50;
            }else if (estado.piecesAtHome(c)==3){
                puntuacion_oponente -= 250;
            }

            /*if (estado.isWall(estado.getBoard().getPiece(c, j).get_box())==c){-> esto EMPEORA
                puntuacion_oponente+=5; 
            }*/
            /*if (estado.isHornMove()){
                puntuacion_oponente+=10;
            }else if (estado.isShockMove()){
                puntuacion_oponente+=10;
            }else if (estado.isStarMove()){
                puntuacion_oponente+=10;
            }*/
            //le restamos la distancia pq quiero q se le de preferencia al que este mas cerca de la meta
            puntuacion_oponente -=estado.distanceToGoal(c,estado.getBoard().getPiece(c, j).get_box());
        }
    }

    
    for (int i = 0; i < current_dices_especiales_oponente.size(); i++)
    {
        if (current_dices_especiales_oponente[i]==banana){  //platano
            puntuacion_oponente += 2;
        }if (current_dices_especiales_oponente[i]==mushroom){  //champiñon
            puntuacion_oponente += 7;
        }if (current_dices_especiales_oponente[i]==red_shell){  //Caparazón rojo
            puntuacion_oponente += 30;
        }if (current_dices_especiales_oponente[i]==blue_shell){  //Caparazón azul
            puntuacion_oponente += 50;
        }if (current_dices_especiales_oponente[i]==horn){  //Bocina
            puntuacion_oponente += 30;
        }if (current_dices_especiales_oponente[i]==bullet){  //Bala
            puntuacion_oponente += 35;
        }if (current_dices_especiales_oponente[i]==shock){  //Rayo
            puntuacion_oponente += 50;
        }if (current_dices_especiales_oponente[i]==boo){  //Boo
            puntuacion_oponente += 20;
        }if (current_dices_especiales_oponente[i]==star){  //Estrella
            puntuacion_oponente += 45;
        }if (current_dices_especiales_oponente[i]==mega_mushroom){  //Megachampiñón
            puntuacion_oponente += 40;
        }
    }

    // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
    return (puntuacion_jugador - puntuacion_oponente);

}

