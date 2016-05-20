#include "fieldview.hpp"

#include <sstream>

#include "evaluation.h"

using namespace std;

/// ####################################
///     Constructeurs et destructeurs
/// ####################################
//@{

FieldWidget::FieldWidget(Field* _field, unsigned _serveDistance):
    QWidget(), field(_field), serveDistance(_serveDistance)
{
    buffer = new QImage;
    color = new QColor(Qt::white);
    bufferPainter= new QPainter;
    rubber = NULL;

    setMinimumSize(field->get_width()*4, field->get_height()*4);
}

FieldWidget::~FieldWidget()
{
    delete field;
    delete buffer;
    delete color;
    delete bufferPainter;

    delete rubber;
}

//@}
///###################
///		Setters
///###################

void FieldWidget::setColor(Colors colorIndice)
{
    switch(colorIndice){
    case LightBlue:
        this->color->setRgb(150,190,220);
        break;
    case DarkBlue:
        this->color->setRgb(50,90,160);
        break;
    case Black:
        this->color->setRgb(00,00,00);
        break;
    case Gray:
        this->color->setRgb(65,65,65);
        break;
    case Red:
        this->color->setRgb(255,0,0);
        break;
    case White :
    default :
        this->color->setRgb(255, 255, 255);
    }
}

/// ####################
///		Affichages
/// ####################
//@{

void FieldWidget::drawCell(int colonne, int ligne)
{
    QPen pe;
//    pe.setWidth(4);
//    pe.setBrush(QColor(0,0,0));
    bufferPainter->setPen(pe);
    bufferPainter->setBrush(*color);
    bufferPainter->fillRect(colonne, ligne, 1, 1, *(color));
    #if DEBUG_TMATRICE
    cout <<"draw cell ; ";
    #endif
}

void FieldWidget::drawList(const std::list<Coordinates> &list_coordinates){

    for( const Coordinates& coord : list_coordinates){
        drawCell(coord.col, coord.row);
    }
}

void FieldWidget::drawField()
{
    if (buffer->isNull())
        cerr<< "Impossible de dessiner, image vide"<< endl;
    bufferPainter->begin(buffer);

    Coordinates& coord= Field::first();
    do {
        State state= field->at(coord);

        // Cas d'une parcelle exploitable
        if( state == is_usable){
            setColor(White);
            drawCell(coord.col, coord.row);
        }
        // Cas d'une parcelle non exploitable
        if( state == is_unusable){
            setColor(LightBlue);
            drawCell(coord.col, coord.row);
        }
        // Cas d'une route
        else if(state == is_road){
            setColor(Gray);
            drawCell(coord.col, coord.row);
        }
        // Cas d'une entrée/sortie
        else if(state == is_in_out){
            setColor(Red);
            drawCell(coord.col, coord.row);
        }
        else if (state == is_undefined){
            setColor(Black);
            cout << "Dessin d'une case non définie"<< endl;
            drawCell(coord.col, coord.row);
        }
    } while (field->nextCoordinates(&coord));
    delete &coord;

    // Dessin des cellules sélectionnées
    setColor(DarkBlue);
    drawList(selecteds);

    bufferPainter->end();
}

void FieldWidget::drawChanged()
{
    bufferPainter->begin(buffer);

    setColor(Red);
//	drawList(forest->getBurned());
//	forest->clearBurned();
    bufferPainter->end();
}

// Test perf
#if PERF_REDRAW
int num_redraw= 0;
#endif

void FieldWidget::redraw()
{
    #if PERF_REDRAW
    ++num_redraw;
    cout << "test redraw firewidget"<< num_redraw<< endl;
    #endif

    if (!buffer->isNull()){
        delete(buffer);
    }
    buffer = new QImage(field->get_width(), field->get_height(), QImage::Format_ARGB32);

    drawField();
    drawChanged();

    update();
}

//@}
/// ########################
/// 	Initialisations
/// ########################

LoadWindow* FieldWidget::createProgressWindow() const
{
    LoadWindow* progressWindow= new LoadWindow();
    return progressWindow;
}

/// #################################
/// 	Gestion des sauvegardes
/// #################################

bool FieldWidget::trySaveImage(QString filename) const
{
    if ( !buffer->isNull() ){
        // TEST verifier que la taille est correcte
        QImage tmp= buffer->scaled(tailleCell*field->get_width(), tailleCell*field->get_height());
        tmp.save(filename);
        return true;
    }
    else{
        #if DEBUG_SAVE
        cout << "Impossible de sauvegarder l'image, de forêt ouverte !"<< endl;
        #endif
        return false;
    }
}

/// ##################
/// 	Events
/// ##################
//@{

bool FieldWidget::tryAddRoadOnParcel(const Coordinates &pos)
{
    if (field->at(pos) == is_road) {
        return false;
    } else {
        if (field->at(pos) != is_in_out) {
            field->add_road(pos);
            field->resetUsables(serveDistance);
        }
        return true;
    }
}

void FieldWidget::clicInOut(const Coordinates &pos)
{
    if (field->at(pos) != is_in_out) {
        if (field->tryAdd_in_out(pos)) field->resetUsables(serveDistance);
    } else {
        field->add_undefined(pos);
        field->resetUsables(serveDistance);
    }

    redraw();
    update();
}

void FieldWidget::clicRoad(const Coordinates &pos)
{
    if ( !tryAddRoadOnParcel(pos) ) {
        field->add_undefined(pos);
        field->resetUsables(serveDistance);
    }

    redraw();
    update();
}

void FieldWidget::moveRoad(const Coordinates &pos)
{
    tryAddRoadOnParcel(pos);

    redraw();
    update();
}

void FieldWidget::selectParcel(const Coordinates &pos)
{
    list<Coordinates>::iterator it= std::find(selecteds.begin(), selecteds.end(), pos);
    if (it != selecteds.end()) {
        selecteds.erase(it);
    } else if (field->at(pos) == is_usable) {
        selecteds.push_back(pos);
        if (selecteds.size() > 2){
            selecteds.pop_front();
        }
        if (selecteds.size() == 2) {
            Evaluation eval(*field, Parameters(serveDistance, 1)); // TODO voir comment on gère la taille de la route
            eval.initSizeNeighbourhood();
            unsigned by_roads_distance= eval.parcelsRoadDistance(selecteds.front(), selecteds.back());
            cout << "Distance entre "<< selecteds.front() <<" et "<< selecteds.back()<< " : "<< by_roads_distance<< endl;
        }
    }

    redraw();
    update();
}


void FieldWidget::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPainter paint(this);

    paint.scale(tailleCell, tailleCell);
    paint.drawImage(1, 1, *buffer);
    paint.scale(1.0/((float)tailleCell), 1.0/((float)tailleCell));
    for (unsigned x= 1; x <= field->get_width(); ++x) {
        float posX= x*tailleCell;
        paint.drawLine(posX, 0, posX, (field->get_height() +1)*tailleCell);
        ostringstream convert;
        convert<< (x-1);
        paint.drawText(QRect(posX +3, 2, tailleCell, tailleCell), QString::fromStdString(convert.str()));
    }

    for (unsigned y= 1; y <= field->get_height(); ++y) {
        float posY= y*tailleCell;
        paint.drawLine(0, posY, (field->get_width() +1) *tailleCell, posY);
        ostringstream convert;
        convert<< (y-1);
        paint.drawText(QRect(2,posY +2, tailleCell, tailleCell), QString::fromStdString(convert.str()));
    }
}

void FieldWidget::resizeEvent(QResizeEvent* event)
{
    #if PERF_RESIZE
    cout << "test resizeEvent firewidget"<< endl;
    #endif

    int nbCol= field->get_width() +1;
    int nbRow= field->get_height()+1;
    tailleCell = min (event->size().width() / (float)nbCol , event->size().height() / (float)nbRow);

    #if DEBUG_CURRENT
// 	cout << "test apres resize dans resizeEvent (ligne 488 firewidget)"<< endl;
    #endif

    #if DEBUG_DIMENSION
    cout << nbCol<< " / "<< nbRow<< endl;
    cout << "tW: "<< event->size().width()<< " tH: "<< event->size().height()<< endl;
    cout << "taille Cellule : "<< tailleCell<< endl;
    cout << endl;
    #endif

    redraw();
}

void FieldWidget::mousePressEvent(QMouseEvent* event)
{
    unsigned col= event->pos().x() / tailleCell -1;
    unsigned row= event->pos().y() / tailleCell -1;
    Coordinates cell_clic(col, row);

    if (field->contains(cell_clic)) {
        if (event->button()==Qt::LeftButton)
        {
            clicRoad(cell_clic);
        }
        else if (event->button()==Qt::MiddleButton)
        {
            selectParcel(cell_clic);
    ///		initRubber(event);
        }
        else if (event->button()==Qt::RightButton)
        {
            clicInOut(cell_clic);
        }

        drawChanged();
        update();
    }
}

void FieldWidget::mouseMoveEvent(QMouseEvent* event)
{
    unsigned col= event->pos().x() / tailleCell -1;
    unsigned row= event->pos().y() / tailleCell -1;
    Coordinates cell_mouse(col, row);

    if (field->contains(cell_mouse)) {
        if (event->buttons().testFlag(Qt::LeftButton) )
        {
            moveRoad(cell_mouse);
        }
        else if (event->buttons().testFlag(Qt::MiddleButton) )
        {}
        else if (event->buttons().testFlag(Qt::RightButton) ){
    //        if(rubber) {
    //			rubber->setGeometry(QRect(origin,event->pos()).normalized());
    //        }
        }
    }

    drawChanged();
    update();
}

//void FieldWidget::initRubber(QMouseEvent* event){
//	origin = event->pos();

//	if(!rubber)
//		rubber = new QRubberBand(QRubberBand::Rectangle, this);

//	rubber->setGeometry(QRect(origin, QSize(0,0)));
//	rubber->show();
//}

//void FieldWidget::mouseReleaseEvent(QMouseEvent* event)
//{
//    QWidget::mouseReleaseEvent(event);
//	if(rubber){
//		rubber->hide();
//		// Sauvegarde des points du rubber pour parcours de la matrice
//		depart.setX(rubber->x());
//		depart.setY(rubber->y());
//		/* Vérification du point d'origine du rubber
//		 * Celui étant toujours le point le plus en haut à gauche, il faut simplement vérifier
//		 * qu'il n'est pas en dehors du cadre, auquel cas nous ramenons la (les) coordonnée(s) concernée(s)
//		 * à 0.
//		 */
//		arrivee.setX(rubber->width()+depart.x());
//		arrivee.setY(rubber->height()+depart.y());
//		if(depart.x() < 0){
//			depart.setX(0);
//		}
//		if(depart.y() < 0){
//			depart.setY(0);
//		}


//		if(arrivee.x() > size().width() ){
//			arrivee.setX(size().width());
//		}
//		if(arrivee.y() > size().height() ){
//			arrivee.setY(size().height());
//		}

//		#if DEBUG_SELECT
//		cout << "Taille du widget : " << this->size().width() << "; " << this->size().height()<< endl;
//		cout << "Coordonnée de l'origine : " << rubber->x() << "; " << rubber->y() << endl;
//		cout << "Coordonnée de départ : " << depart.x()<< ";" << depart.y() << endl;
//		cout << "Coordonnée de l'arrivée : " << arrivee.x()<< ";" << arrivee.y() << endl;
//		cout << "Taille de la zone de selection : " <<	arrivee.x() - depart.x() << ";" << arrivee.y() - depart.y() << endl;
//		#endif
//		// Emission du signal pour récupérer l'action à effectuer par firescreen
////		emit releaseSignal(); // TODO signaux
//	}

//}

//@}
/// ############
///    Slots
/// ############
//@{

//void FieldWidget::actionReceived(int action_id)
//{
//    // Transformation des QPoints depart et arrivée en coordonnée cellulaire
//    int xDep = depart.x() / tailleCell;
//    int yDep = depart.y() / tailleCell;

//    unsigned xArr = arrivee.x() / tailleCell;

//    if (xArr > field->get_width()) xArr= field->get_width();

//    unsigned yArr = arrivee.y() / tailleCell;

//    if (yArr> field->get_height())	yArr= field->get_height();


//    #if DEBUG_RETARD
//    cout << "Coordonnées en cellule de départ : " << xDep << ";" << yDep << endl;
//    cout << "Coordonnées en cellule d'arrivée : " << xArr << ";" << yArr << endl;
//    #endif

//    // Appel à une fonction de forêt qui parcours la zone et effectue l'action


////    if(action_id == CUT){
////    }else if( action_id == DELAY){
////    }else cerr<< "mauvais index d'action clic droit"<< endl;

//    drawChanged();
//    update();
//}

//@}
