#include "fieldview.hpp"

#include <sstream>

#include "evaluation.h"

using std::cout; using std::cerr; using std::cout; using std::endl;

/// ####################################
///     Constructeurs et destructeurs
/// ####################################
//@{

FieldWidget::FieldWidget(Field* _field, unsigned _serveDistance):
    QWidget(), field(_field), serveDistance(_serveDistance), modified_ES(false), has_evaluation(false)
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
        this->color->setRgb(160,200,250);
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

void FieldWidget::setColor(int r, int g, int b, int a)
{
    color->setRgb(r, g, b, a);
}

void FieldWidget::update_field(FieldEvaluation *_field)
{
    field= _field;
    has_evaluation= true;
}

void FieldWidget::set_unmodified_ES()
{
    modified_ES= false;
}

/// ####################
///		Affichages
/// ####################
//@{

void FieldWidget::drawCell(int colonne, int ligne)
{
    QPen pe;
    pe.setColor(*color);
    bufferPainter->setPen(pe);
    bufferPainter->drawPoint(colonne, ligne);
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

void FieldWidget::drawHotmapField()
{
    field->resetUsables(serveDistance);
    if (buffer->isNull()) cerr<< "Impossible de dessiner, image vide"<< endl;

    bufferPainter->begin(buffer);

    Coordinates& coord= Field::first();
    do {
        State state= field->at(coord);

        // Cas d'une parcelle exploitable
        if( state == is_usable) {
            FieldEvaluation* field_eval= (FieldEvaluation *)(field);
            if (!has_evaluation || field_eval == nullptr) {
                cerr<< "La surface n'a pas d'évaluation"<< endl;
                setColor(White);
            } else {
                float sum= 0.0;
                unsigned nb= 0;
                field_eval->initRoadDistances();
                float avg_avg_ratio= field_eval->evaluateRatio();

                Coordinates& other= Field::first();
                do {
                    if (field_eval->at(other) == is_usable && !(other == coord)) {
                        float ratio= field_eval->manhattanRatioBetween2Parcels(coord, other);
                        sum+= ratio;
                        ++nb;
                    }

                } while (field_eval->nextCoordinates(&other));
                delete &other;

                float coord_avg= sum / (float)nb;
#if DEBUG_HOTMAP
                cout << "\tSomme : "<< sum<< ", moy= "<< coord_avg<< endl;
#endif

                unsigned green_quant= 200;
                float delta_ratio= coord_avg - avg_avg_ratio;
                unsigned red_quant;
                unsigned blue_quant= 110;
                if (delta_ratio < 0) {
                    if (delta_ratio +2 < 0) {
                        blue_quant= 170;
                        red_quant= 0;
                    } else {
                        red_quant= 110 * (delta_ratio +2);
                    }
                    green_quant= 255;
                } else {
                    /// La valeur 400 est arbitraire
                    red_quant= 400* (delta_ratio);
                    if (red_quant > 255) {
                        if (red_quant > 255 + green_quant) {
                            if (red_quant > 255 + green_quant + blue_quant) {
                                blue_quant= 0;
                            } else blue_quant-= (red_quant -510);

                            red_quant= 255 + green_quant;
                        }
                        green_quant-= (red_quant -255);
                        red_quant= 255;
                    }
                }
#if DEBUG_HOTMAP
                cout << "Ecart avec moyenne : "<< delta_ratio<< " ; "
                    << "Q. rouge : "<< red_quant<< ", Q. vert : "<< green_quant<< ", Q. bleu : "<< blue_quant<< endl;
#endif
                setColor(red_quant, green_quant, blue_quant);
            }

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
    update();
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
    // Tente d'ajouter une E/S
    if (field->at(pos) != is_in_out) {
        if (field->tryAdd_in_out(pos)) {
            field->resetUsables(serveDistance);
            modified_ES= true;
        }
    // Supprime l'E/S présente
    } else {
        field->add_undefined(pos);
        field->resetUsables(serveDistance);
        modified_ES= true;
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
    std::list<Coordinates>::iterator it= std::find(selecteds.begin(), selecteds.end(), pos);
    if (it != selecteds.end()) {
        selecteds.erase(it);
    } else if (field->at(pos) == is_usable) {
        selecteds.push_back(pos);
        if (selecteds.size() > 2){
            selecteds.pop_front();
        }
        if (selecteds.size() == 2) {
            FieldEvaluation* eval;
            if (has_evaluation) {
                eval= dynamic_cast<FieldEvaluation*>(field);
                if (eval == NULL) cerr<< "La surface devrait posséder une évaluation, mais ce n'est pas le cas (échec du dynamic_cast)"<< endl;
            } else {
                eval= new FieldEvaluation(*field, Parameters(serveDistance, 1)); // TODO voir comment on gère la taille de la route
            }
            if (!eval->road_distances_are_initiated) eval->initRoadDistances();

            unsigned by_roads_distance= eval->parcelsRoadDistance(selecteds.front(), selecteds.back());
            cout << "Distance entre "<< selecteds.front() <<" et "<< selecteds.back()<< " : "<< by_roads_distance<< endl;

            if (has_evaluation) {
                delete eval;
            }
        }
    }

    redraw();
    update();
}


void FieldWidget::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPainter paint(this);

    FieldEvaluation* field_eval= (FieldEvaluation *)(field);
    if (has_evaluation && field_eval != nullptr) {
        std::ostringstream usables;
        usables << field_eval->get_nbUsables();
        paint.drawText(QRect(1, 1, tailleCell, tailleCell), QString::fromStdString(usables.str()));

        std::ostringstream access;
        access << field_eval->get_avgAccess();
        paint.drawText(QRect(1, 12, tailleCell, tailleCell), QString::fromStdString(access.str()));
    }

    paint.scale(tailleCell, tailleCell);
    paint.drawImage(1, 1, *buffer);
    paint.scale(1.0/((float)tailleCell), 1.0/((float)tailleCell));
    for (unsigned x= 1; x <= field->get_width(); ++x) {
        float posX= x*tailleCell;
        paint.drawLine(posX, 0, posX, (field->get_height() +1)*tailleCell);
        std::ostringstream convert;
        convert<< (x-1);
        paint.drawText(QRect(posX +3, 2, tailleCell, tailleCell), QString::fromStdString(convert.str()));
    }

    for (unsigned y= 1; y <= field->get_height(); ++y) {
        float posY= y*tailleCell;
        paint.drawLine(0, posY, (field->get_width() +1) *tailleCell, posY);
        std::ostringstream convert;
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
    tailleCell = std::min(event->size().width() / (float)nbCol , event->size().height() / (float)nbRow);

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
    Coordinates cell_clic((int)col, (int)row);

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

    update();
}

//@}
/// ############
///    Slots
/// ############
//@{

//@}
