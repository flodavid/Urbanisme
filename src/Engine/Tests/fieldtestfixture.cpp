#include "fieldtestfixture.h"

using namespace std;

void FieldTestFixture::setUp()
{
    field= new Field(20, 20);
    
    coord_in= new Coordinates(5, 12);
    coord_border_bot= new Coordinates(5, 19);
    coord_border_top= new Coordinates(12, 0);
    coord_border_left= new Coordinates(0, 4);
    coord_border_right= new Coordinates(19, 18);
    
    coord_out= new Coordinates(-1, 12);

    // Affichage
    cout << "Coordinates of test for field : " << (*coord_in)
    << " and "<< (*coord_border_bot)
    << " and "<< (*coord_border_top)
    << " and "<< (*coord_border_left)
    << " and "<< (*coord_border_right)
    << " and out : "<< (*coord_out)<< endl;
}

void FieldTestFixture::tearDown()
{
    CppUnit::TestFixture::tearDown();
    
    delete field;
    
    delete coord_in;
    delete coord_border_bot;
    delete coord_border_top;
    delete coord_border_left;
    delete coord_border_right;
    
    delete coord_out;
}


void FieldTestFixture::test_containsValids()
{
    CPPUNIT_ASSERT(field->contains(*coord_in));
    CPPUNIT_ASSERT(field->contains(*coord_border_bot));
    CPPUNIT_ASSERT(field->contains(*coord_border_top));
    CPPUNIT_ASSERT(field->contains(*coord_border_left));
    CPPUNIT_ASSERT(field->contains(*coord_border_right));
}


void FieldTestFixture::test_containsInvalids()
{
    CPPUNIT_ASSERT(! field->contains(*coord_out));
}

// void FieldTestFixture::test_parcelsStatus(){
//     cout << "vérification que ("<< j<< ","<< i<< ") est une route";
//     if (field.contains(j, i) && (j != coord.col || i != coord.row))
// 	cout << " : "<< (field[Coordinates(j, i)] >= is_road);
//     cout << endl;
// }

void FieldTestFixture::test_servingValids()
{
    clog << "Test validité getServingRoads()"<< endl;
    Field &example = *field;
    example.deleteOldMatrix();
    example.createExample();
    
    unsigned serve_dist= 2;
    
    field->setUsables(serve_dist);
    
    // route (5,12)
    list<Coordinates> serving_in;
	serving_in.push_back(Coordinates(4, 12));
	serving_in.push_back(Coordinates(6, 12));
    
    // parcelle (5, 19)
    list<Coordinates> serving_bot;
	serving_bot.push_back(Coordinates(5, 17));
	
    CPPUNIT_ASSERT_EQUAL(serving_bot, *(field->getServingRoads(*coord_border_bot, serve_dist)));
	
    // parcelle (12, 2)
    list<Coordinates> serving_top;
	serving_top.push_back(Coordinates(12, 2));
	
    CPPUNIT_ASSERT_EQUAL(serving_top, *(field->getServingRoads(*coord_border_top, serve_dist)));
    
    // parcelle (0, 4)
    list<Coordinates> serving_left;
	serving_left.push_back(Coordinates(0, 2));
	
    CPPUNIT_ASSERT_EQUAL(serving_left, *(field->getServingRoads(*coord_border_left, serve_dist)));
	
    // parcelle (19, 18)
    list<Coordinates> serving_right;
	serving_right.push_back(Coordinates(19, 17));
	
    CPPUNIT_ASSERT_EQUAL(serving_right, *(field->getServingRoads(*coord_border_right, serve_dist)));
}

