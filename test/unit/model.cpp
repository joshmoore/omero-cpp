/*
 *   $Id$
 *
 *   Copyright 2007 Glencoe Software, Inc. All rights reserved.
 *   Use is subject to license terms supplied in LICENSE.txt
 *
 */

#include <boost_fixture.h>
#include <omero/model/ProjectI.h>
#include <omero/model/ProjectDatasetLinkI.h>
#include <omero/model/DatasetI.h>
#include <omero/model/DatasetImageLinkI.h>
#include <omero/model/ImageI.h>
#include <omero/model/PixelsI.h>
#include <omero/model/ChannelI.h>
#include <omero/model/TagAnnotationI.h>
#include <omero/model/GroupExperimenterMapI.h>

using namespace omero::rtypes;
using namespace omero::model;
using namespace omero;
using namespace std;

BOOST_AUTO_TEST_CASE( DetailsPtrIsNull )
{
    DetailsIPtr p;
    BOOST_CHECK( ! p );
}

BOOST_AUTO_TEST_CASE( Virtual )
{
  ImagePtr img = new ImageI();
  ImageIPtr imgI = new ImageI();
  img->unload();
  imgI->unload();
}

BOOST_AUTO_TEST_CASE( Toggle )
{
  Fixture f;
  PixelsIPtr pix = new PixelsI();
  BOOST_CHECK( pix->sizeOfSettings() >= 0 );
  pix->unloadCollections();
  BOOST_CHECK( pix->sizeOfSettings() < 0 );
}

BOOST_AUTO_TEST_CASE( SimpleCtor )
{
  Fixture f;
  ImageIPtr img = new ImageI();
  BOOST_CHECK( img->isLoaded() );
  BOOST_CHECK( img->sizeOfPixels() >= 0 );
}

BOOST_AUTO_TEST_CASE( UnloadedCtor )
{
  Fixture f;
  ImageIPtr img = new ImageI(rlong(1),false);
  BOOST_CHECK( !(img->isLoaded()) );
  BOOST_CHECK_THROW( img->sizeOfDatasetLinks(), omero::UnloadedEntityException );
}

BOOST_AUTO_TEST_CASE( UnloadCheckPtr )
{
  Fixture f;
  ImageIPtr img = new ImageI();
  BOOST_CHECK( img->isLoaded() );
  // operator bool() is overloaded
  BOOST_CHECK( img->getDetails() ); // details are auto instantiated
  BOOST_CHECK( ! img->getName() ); // no other single-valued field is
  img->unload();
  BOOST_CHECK( !img->isLoaded() );
  BOOST_CHECK_THROW( img->getDetails(), omero::UnloadedEntityException );
}

BOOST_AUTO_TEST_CASE( UnloadField )
{
  Fixture f;
  ImageIPtr img = new ImageI();
  BOOST_CHECK( img->getDetails() );
  img->unloadDetails();
  BOOST_CHECK( ! img->getDetails() );
}

BOOST_AUTO_TEST_CASE( Sequences )
{
  Fixture f;
  ImageIPtr img = new ImageI();
  BOOST_CHECK( img->sizeOfAnnotationLinks() == 0 );
  img->unloadAnnotationLinks();
  img->unload();
  BOOST_CHECK_THROW( img->sizeOfAnnotationLinks(), omero::UnloadedEntityException );
}

BOOST_AUTO_TEST_CASE( Accessors )
{
  Fixture f;
  RStringPtr name = rstring("name");
  ImageIPtr img = new ImageI();
  BOOST_CHECK( !img->getName() );
  img->setName( name );
  BOOST_CHECK( img->getName() );
  RStringPtr str = img->getName();
  BOOST_CHECK( str->getValue() == "name" );
  BOOST_CHECK( str == name );

  img->setName(rstring("name2"));
  BOOST_CHECK( img->getName()->getValue() == "name2" );
  BOOST_CHECK( img->getName() );

  img->unload();
  BOOST_CHECK_THROW( img->getName(), omero::UnloadedEntityException );
  
}

BOOST_AUTO_TEST_CASE( UnloadedAccessThrows )
{
  Fixture f;
  ImageIPtr unloaded = new ImageI(rlong(1),false);
  BOOST_CHECK_THROW( unloaded->getName(), omero::UnloadedEntityException );
}

BOOST_AUTO_TEST_CASE( Iterators )
{
  Fixture f;

  DatasetIPtr d = new DatasetI();
  ImageIPtr image = new ImageI();
  image->linkDataset(d);
  ImageDatasetLinksSeq::iterator it= image->beginDatasetLinks();
  int count = 0;
  for (;it != image->endDatasetLinks(); ++it) {
    count++;
  }
  BOOST_CHECK( count == 1 );
}

BOOST_AUTO_TEST_CASE( ClearSet )
{
  Fixture f;
  ImageIPtr img = new ImageI();
  BOOST_CHECK( img->sizeOfPixels() >= 0 );
  img->addPixels( new PixelsI() );
  BOOST_CHECK( 1==img->sizeOfPixels() );
  img->clearPixels();
  BOOST_CHECK( img->sizeOfPixels() >= 0 );
  BOOST_CHECK( 0==img->sizeOfPixels() );
}

BOOST_AUTO_TEST_CASE( UnloadSet )
{
  Fixture f;
  ImageIPtr img = new ImageI();
  BOOST_CHECK( img->sizeOfPixels() >= 0 );
  img->addPixels( new PixelsI() );
  BOOST_CHECK( 1==img->sizeOfPixels() );
  img->unloadPixels();
  BOOST_CHECK( img->sizeOfPixels() < 0 );
  // Can't check size BOOST_CHECK( 0==img->sizeOfPixels() );
}

BOOST_AUTO_TEST_CASE( RemoveFromSet )
{
  Fixture f;
  PixelsIPtr pix = new PixelsI();
  ImageIPtr img = new ImageI();
  BOOST_CHECK( img->sizeOfPixels() >= 0 );

  img->addPixels( pix );
  BOOST_CHECK( 1==img->sizeOfPixels() );

  img->removePixels( pix );
  BOOST_CHECK( 0==img->sizeOfPixels() );
}

BOOST_AUTO_TEST_CASE( LinkGroupAndUser )
{
  Fixture f;

  ExperimenterIPtr user = new ExperimenterI();
  ExperimenterGroupIPtr group = new ExperimenterGroupI();
  GroupExperimenterMapIPtr map = new GroupExperimenterMapI();

  map->setId( rlong(1) );
  map->link(group,user);
  user->addGroupExperimenterMapToBoth( map, false );
  group->addGroupExperimenterMapToBoth( map, false );

  typedef ExperimenterGroupExperimenterMapSeq::iterator egm_it; 
  egm_it beg = user->beginGroupExperimenterMap();
  egm_it end = user->endGroupExperimenterMap();
  int count = 0 ;
  for( ; beg != end; beg++ ) {
    ++count;
  }
  BOOST_CHECK( count == 1 );

}

BOOST_AUTO_TEST_CASE( LinkViaMap )
{
  Fixture f;
  ExperimenterIPtr user = new ExperimenterI();
  user->setFirstName(rstring("test"));
  user->setLastName(rstring("user"));
  user->setOmeName(rstring("UUID"));

  // possibly setOmeName() and setOmeName(string) ??
  // and then don't need omero/types.h

  ExperimenterGroupIPtr group = new ExperimenterGroupI();
  // TODOuser->linkExperimenterGroup(group);
  GroupExperimenterMapIPtr map = new GroupExperimenterMapI();
  map->setParent( group );
  map->setChild( user );
}

BOOST_AUTO_TEST_CASE( LinkingAndUnlinking )
{
  Fixture f;

  DatasetImageLinkIPtr dil;

  DatasetIPtr d = new DatasetI();
  ImageIPtr   i = new ImageI();

  d->linkImage(i);
  BOOST_CHECK( d->sizeOfImageLinks() == 1 );
  d->unlinkImage(i);
  BOOST_CHECK( d->sizeOfImageLinks() == 0 );

  d = new DatasetI();
  i = new ImageI();
  d->linkImage(i);
  BOOST_CHECK( i->sizeOfDatasetLinks() == 1 );
  i->unlinkDataset(d);
  BOOST_CHECK( d->sizeOfImageLinks() == 0 );

  d = new DatasetI();
  i = new ImageI();
  dil = new DatasetImageLinkI();
  dil->link(d,i);
  d->addDatasetImageLinkToBoth(dil, false);
  BOOST_CHECK( d->sizeOfImageLinks() == 1 );
  BOOST_CHECK( i->sizeOfDatasetLinks() == 0 );

}

BOOST_AUTO_TEST_CASE( UnloadedEntityTermination ) {

  Fixture f;

  ProjectDatasetLinkIPtr pDL = new ProjectDatasetLinkI();
  ProjectIPtr p = new ProjectI();
  DatasetIPtr d = new DatasetI();
  //pDL->link(p,d);
  d->unload();
  omero::model::IObjectPtr             theChild =
  omero::model::IObjectPtr::dynamicCast(pDL->getChild());
  //cout << "theChild is: " << theChild << "... pDS" << endl;
  omero::model::DatasetIPtr            pDS =
  omero::model::DatasetIPtr::dynamicCast(theChild);

}

BOOST_AUTO_TEST_CASE( PrimaryPixels ) {

    Fixture f;

    ImageIPtr i = new ImageI();

    BOOST_CHECK_EQUAL( true, i->isPixelsLoaded() );
    BOOST_CHECK_EQUAL( 0, i->sizeOfPixels() );
    bool called = false;
    ImagePixelsSeq::iterator beg = i->beginPixels();
    ImagePixelsSeq::iterator end = i->endPixels();
    while (beg != end) {
        called = true;
        beg++;
    }
    BOOST_CHECK_EQUAL( false, called );


    PixelsIPtr p = new PixelsI();
    i->addPixels( p );

    BOOST_CHECK_EQUAL( true, i->isPixelsLoaded() );
    BOOST_CHECK_EQUAL( 1, i->sizeOfPixels() );
    BOOST_CHECK_EQUAL( p, i->beginPixels()[0] );
    beg = i->beginPixels();
    end = i->endPixels();
    while (beg != end) {
        called = true;
        beg++;
    }
    BOOST_CHECK_EQUAL( true, called );


    i->unloadPixels();

    BOOST_CHECK_EQUAL( false, i->isPixelsLoaded() );
    BOOST_CHECK_EQUAL( -1, i->sizeOfPixels() );
    try {
        i->beginPixels();
        BOOST_FAIL( "Should have thrown an exception ");
    } catch (const std::exception& ex) {
        // ok
    }

}

BOOST_AUTO_TEST_CASE( OrderedCollectionsTicket2547 ) {
    PixelsPtr pixels = new PixelsI();
    ChannelPtr channel0 = new ChannelI();
    ChannelPtr channel1 = new ChannelI();
    ChannelPtr channel2 = new ChannelI();
    pixels->addChannel(channel0);
    BOOST_CHECK_EQUAL(1, pixels->sizeOfChannels());
    ChannelPtr old = pixels->setChannel(0, channel1);
    BOOST_CHECK_EQUAL(old, channel0);
    BOOST_CHECK_EQUAL(1, pixels->sizeOfChannels());
}
