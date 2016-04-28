#ifndef LOADWINDOW_H
#define LOADWINDOW_H

#include <QtWidgets/QProgressBar>
#include <QtWidgets/QWidget>

#include "../Engine/loadprogress.h"

/**
 * Permet d'afficher une fenêtre avec une barre de progression pour les chargements
 * Utilise la classe abstraite LoadProgress
 * @author Florian
 */
class LoadWindow : public LoadProgress, QWidget
{
private:
	QProgressBar* PB_load;
	
public:
	/**
	 * Crée une fenêtre contenant un titre et une barre de progression, puis la montre
	 * @author Florian
	 */
	LoadWindow();
	virtual ~LoadWindow();
	
	/**
	 * Met à jour la valeur de progression
	 * @author Florian
	 * @param pourcentage nouveau pourcentage de progression, 
	 */
	virtual void setProgress(int pourcentage);
	/**
	 * Ferme la fenetre de progression
	 * @author Florian
	 */
	virtual void closeProgress();
};

#endif // LOADWINDOW_H
