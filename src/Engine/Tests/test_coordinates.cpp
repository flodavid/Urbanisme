#include "test_coordinates.h"

using namespace std;

void CoordinatesTestFixture::setUp() {
    
    coord1= new Coordinates(0,0);
    coord2= new Coordinates(4,8);
    
    expected_manhattan= 12;
    expected_euclid = 8.94427191;
    
    cout << "Coordinates of test : " << (*coord1)<< " and "<< (*coord2)<< endl;
}

void CoordinatesTestFixture::tearDown() {

}

void CoordinatesTestFixture::test_calcManhattan()
{
    int manhattan_dist= coord1->manhattanDistance(*coord2);
    CPPUNIT_ASSERT(manhattan_dist == expected_manhattan);
}

void CoordinatesTestFixture::test_calcEuclidean()
{
    float euclidean_dist= coord1->euclideanDistance(*coord2);
    CPPUNIT_ASSERT( (expected_euclid - euclidean_dist) <=  0.001 );
}
