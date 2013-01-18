/*
 *   $Id$
 *
 *   Copyright 2007 Glencoe Software, Inc. All rights reserved.
 *   Use is subject to license terms supplied in LICENSE.txt
 *
 */

#include <omero/model/PermissionsI.h>
#include <omero/fixture.h>

TEST( PermsTest, Perm1 )
{
  omero::model::PermissionsIPtr p = new omero::model::PermissionsI();

  // The default
  ASSERT_TRUE( p->isUserRead() );
  ASSERT_TRUE( p->isUserWrite() );
  ASSERT_TRUE( p->isGroupRead() );
  ASSERT_TRUE( p->isGroupWrite() );
  ASSERT_TRUE( p->isWorldRead() );
  ASSERT_TRUE( p->isWorldWrite() );

  // All off
  p->setPerm1( 0L );
  ASSERT_TRUE( ! p->isUserRead() );
  ASSERT_TRUE( ! p->isUserWrite() );
  ASSERT_TRUE( ! p->isGroupRead() );
  ASSERT_TRUE( ! p->isGroupWrite() );
  ASSERT_TRUE( ! p->isWorldRead() );
  ASSERT_TRUE( ! p->isWorldWrite() );

  // All on
  p->setPerm1( -1L );
  ASSERT_TRUE( p->isUserRead() );
  ASSERT_TRUE( p->isUserWrite() );
  ASSERT_TRUE( p->isGroupRead() );
  ASSERT_TRUE( p->isGroupWrite() );
  ASSERT_TRUE( p->isWorldRead() );
  ASSERT_TRUE( p->isWorldWrite() );

  // Various swaps
  p->setUserRead(false);
  ASSERT_TRUE( !p->isUserRead() );
  p->setGroupWrite(true);
  ASSERT_TRUE( p->isGroupWrite() );

  // Now reverse each of the above
  p->setUserRead(true);
  ASSERT_TRUE( p->isUserRead() );
  p->setGroupWrite(false);
  ASSERT_TRUE( !p->isGroupWrite() );

}
