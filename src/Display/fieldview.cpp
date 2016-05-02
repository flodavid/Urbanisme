#include "fieldview.hpp"

using namespace std;

/// ####################################
///     Constructeurs et destructeurs
/// ####################################
//@{

FieldWidget::FieldWidget(const Field* _field):
    QWidget(), field(_field)
{	
	buffer = new QImage;
	color = new QColor(Qt::white);
	bufferPainter= new QPainter;
    rubber = NULL;

    setMinimumSize(field->get_width(), field->get_height());
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
    pe.setWidth(4);
    pe.setBrush(QColor(0,0,0));
    bufferPainter->setPen(pe);
    bufferPainter->setBrush(*color);
    bufferPainter->fillRect(colonne, ligne, 1, 1, *(color));
    #if DEBUG_TMATRICE
    cout <<"draw cell ; ";
    #endif
}

void FieldWidget::drawList( list< Coordinates* > * list_coordinates){

    for( const Coordinates* coord : *list_coordinates){
        drawCell(coord->col, coord->row);
    }
    list_coordinates->clear();
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
    update();	// TODO apparemment non utile, update fait resize
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

void FieldWidget::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
	QPainter paint(this);
	paint.scale(tailleCell, tailleCell);
	paint.drawImage(0, 0, *buffer);
}

void FieldWidget::resizeEvent(QResizeEvent* event)
{
	#if PERF_RESIZE
	cout << "test resizeEvent firewidget"<< endl;
	#endif
	
    int nbCol= field->get_width();
    int nbRow= field->get_height();
	tailleCell = min (event->size().width() / (float)nbCol , event->size().height() / (float)nbRow);
	
	// TODO voir comment modifier la taille de de FireWidget sans repasser par resizeEvent, vraiment utile (ajouté pour rubberband, le modifier pour s'adapter?)
// 	resize(tailleCell * nbCol, tailleCell*nbRow); // Fait lagger
	
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
	
	if (event->button()==Qt::LeftButton)
    {}
	else if (event->button()==Qt::MiddleButton)
    {}
	else if (event->button()==Qt::RightButton)
	{
		initRubber(event);
		
		#if DEBUG_IMAGE_COLOR
		QColor* pix= new QColor(pictureForest->pixel(colonne, ligne));
		cout << "qté vert en "<< colonne<< " ; "<< ligne<<" : "<< pix->green();
		cout << "\tqté red : " << pix->red() << " ; bleu : "<< pix->blue()<< endl;
		#endif
	}
	
	drawChanged();
	update();
}

void FieldWidget::initRubber(QMouseEvent* event){
	origin = event->pos();
	
	if(!rubber)
		rubber = new QRubberBand(QRubberBand::Rectangle, this);

	rubber->setGeometry(QRect(origin, QSize(0,0)));
	rubber->show();
}

void FieldWidget::mouseMoveEvent(QMouseEvent* event)
{
//	int colonne= event->x()/tailleCell;
//	int ligne= event->y()/tailleCell;
	
	if (event->buttons().testFlag(Qt::LeftButton) )
    {}
	else if (event->buttons().testFlag(Qt::MiddleButton) )
    {}
	else if (event->buttons().testFlag(Qt::RightButton) ){
        if(rubber) {
			rubber->setGeometry(QRect(origin,event->pos()).normalized());
        }
	}
	
	drawChanged();
	update();
}

void FieldWidget::mouseReleaseEvent(QMouseEvent* event)
{
    QWidget::mouseReleaseEvent(event);
	if(rubber){
		rubber->hide();
		// Sauvegarde des points du rubber pour parcours de la matrice
		depart.setX(rubber->x());
		depart.setY(rubber->y());
		/* Vérification du point d'origine du rubber
		 * Celui étant toujours le point le plus en haut à gauche, il faut simplement vérifier 
		 * qu'il n'est pas en dehors du cadre, auquel cas nous ramenons la (les) coordonnée(s) concernée(s)
		 * à 0.
		 */
		arrivee.setX(rubber->width()+depart.x());
		arrivee.setY(rubber->height()+depart.y());
		if(depart.x() < 0){
			depart.setX(0);
		}
		if(depart.y() < 0){
			depart.setY(0);
		}
		
		
		if(arrivee.x() > size().width() ){
			arrivee.setX(size().width());
		}
		if(arrivee.y() > size().height() ){
			arrivee.setY(size().height());
		}

		#if DEBUG_SELECT
		cout << "Taille du widget : " << this->size().width() << "; " << this->size().height()<< endl;
		cout << "Coordonnée de l'origine : " << rubber->x() << "; " << rubber->y() << endl;
		cout << "Coordonnée de départ : " << depart.x()<< ";" << depart.y() << endl;
		cout << "Coordonnée de l'arrivée : " << arrivee.x()<< ";" << arrivee.y() << endl;
		cout << "Taille de la zone de selection : " <<	arrivee.x() - depart.x() << ";" << arrivee.y() - depart.y() << endl;
		#endif
		// Emission du signal pour récupérer l'action à effectuer par firescreen
//		emit releaseSignal(); // TODO signaux
	}
	
}

//@}
/// ############
///    Slots
/// ############
//@{

//void FieldWidget::actionReceived(int x)
//{
//	// Transformation des QPoints depart et arrivée en coordonnée cellulaire
//	int xDep = depart.x() / tailleCell;
//	int yDep = depart.y() / tailleCell;
	
//    unsigned xArr = arrivee.x() / tailleCell;
	
//    if (xArr > field->get_width()) xArr= field->get_width();
	
//    unsigned yArr = arrivee.y() / tailleCell;
	
//    if (yArr> field->get_height())	yArr= field->get_height();

	
//	#if DEBUG_RETARD
//	cout << "Coordonnée en cellule du départ : " << xDep << ";" << yDep << endl;
//	cout << "Coordonnée en cellule de l'arrivée : " << xArr << ";" << yArr << endl;
//	#endif
	
//	// Appel à une fonction de forêt qui parcours la zone et effectue l'action
	
	
//	if(x == CUT){
//	}else if( x == DELAY){
//	}else cerr<< "mauvais index d'action clic droit"<< endl;
	
//	drawChanged();
//	update();
//}

//@}
