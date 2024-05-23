#!/bin/sh

sudo -u postgres psql postgres -f db-utils/parameters.sql -f db-utils/setup.sql -f schema.sql -f db-utils/auth.sql -f db-utils/toyData.sql  -f db-utils/logSetup.sql -f db-utils/logdb.sql -f db-utils/logAuth.sql -f db-utils/logTrig.sql

#-f triggers.sql  -f tests.sql

