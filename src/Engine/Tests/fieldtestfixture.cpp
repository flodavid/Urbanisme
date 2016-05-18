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
    Field &example = *field;
    example.set_width(20);
    example.set_height(20);
    example.resizeWithDimensions();
    
    example.tryAdd_in_out(9, 0);
    // Colonne Milieu
    example.add_road(9, 1);
    example.add_road(9, 2);
    example.add_road(9, 3);
    example.add_road(9, 4);
    example.add_road(9, 5);
    example.add_road(9, 6);
    example.add_road(9, 7);
    example.add_road(9, 8);
    example.add_road(9, 9);
    example.add_road(9, 10);
    example.add_road(9, 11);
    example.add_road(9, 12);
    example.add_road(9, 13);
    example.add_road(9, 14);
    example.add_road(9, 15);
    example.add_road(9, 16);
    example.add_road(9, 17);
    example.add_road(9, 18);
    // Première Ligne
    example.add_road(0, 2);
    example.add_road(1, 2);
    example.add_road(2, 2);
    example.add_road(3, 2);
    example.add_road(4, 2);
    example.add_road(5, 2);
    example.add_road(6, 2);
    example.add_road(7, 2);
    example.add_road(8, 2);
    // example.add_road(9, 2); déjà fait dans colonne milieu
    example.add_road(10, 2);
    example.add_road(11, 2);
    example.add_road(12, 2);
    example.add_road(13, 2);
    example.add_road(14, 2);
    example.add_road(15, 2);
    example.add_road(16, 2);
    example.add_road(17, 2);
    example.add_road(18, 2);
    example.add_road(19, 2);
    // Deuxième Ligne
    example.add_road(0, 7);
    example.add_road(1, 7);
    example.add_road(2, 7);
    example.add_road(3, 7);
    example.add_road(4, 7);
    example.add_road(5, 7);
    example.add_road(6, 7);
    example.add_road(7, 7);
    example.add_road(8, 7);
    // example.add_road(9, 7); déjà fait dans colonne milieu
    example.add_road(10, 7);
    example.add_road(11, 7);
    example.add_road(12, 7);
    example.add_road(13, 7);
    example.add_road(14, 7);
    example.add_road(15, 7);
    example.add_road(16, 7);
    example.add_road(17, 7);
    example.add_road(18, 7);
    example.add_road(19, 7);
    // Troisième Ligne
    example.add_road(0, 12);
    example.add_road(1, 12);
    example.add_road(2, 12);
    example.add_road(3, 12);
    example.add_road(4, 12);
    example.add_road(5, 12);
    example.add_road(6, 12);
    example.add_road(7, 12);
    example.add_road(8, 12);
    // example.add_road(9, 12); déjà fait dans colonne milieu
    example.add_road(10, 12);
    example.add_road(11, 12);
    example.add_road(12, 12);
    example.add_road(13, 12);
    example.add_road(14, 12);
    example.add_road(15, 12);
    example.add_road(16, 12);
    example.add_road(17, 12);
    example.add_road(18, 12);
    example.add_road(19, 12);
    // Quatrième Ligne
    example.add_road(0, 17);
    example.add_road(1, 17);
    example.add_road(2, 17);
    example.add_road(3, 17);
    example.add_road(4, 17);
    example.add_road(5, 17);
    example.add_road(6, 17);
    example.add_road(7, 17);
    example.add_road(8, 17);
    // example.add_road(9, 17); déjà fait dans colonne milieu
    example.add_road(10, 17);
    example.add_road(11, 17);
    example.add_road(12, 17);
    example.add_road(13, 17);
    example.add_road(14, 17);
    example.add_road(15, 17);
    example.add_road(16, 17);
    example.add_road(17, 17);
    example.add_road(18, 17);
    example.add_road(19, 17);
    
    example.tryAdd_in_out(9, 19);
    
    unsigned serve_dist= 2;
    
    field->defineUsables(serve_dist);
    
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

