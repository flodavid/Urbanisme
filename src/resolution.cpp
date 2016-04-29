#include "resolution.h"

Resolution::Resolution(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth):
    nb_cols(nbCols), nb_rows(nbRows), params(serveDistance, roadsWidth)
{

}


Resolution::Resolution(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth, std::list<Coordinates>& ins_outs):
    nb_cols(nbCols), nb_rows(nbRows), params(serveDistance, roadsWidth)
{
    
}
