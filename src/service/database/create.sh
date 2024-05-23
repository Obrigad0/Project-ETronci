#!/bin/sh

sudo -u postgres psql postgres -f db-utils/parameters.sql -f db-utils/setup.sql -f schema.sql -f db-utils/auth.sql -f db-utils/toyData.sql  -f ../logDatabase/logSetup.sql -f ../logDatabase/logdb.sql -f ../logDatabase/logAuth.sql -f ../logDatabase/logTrig.sql

#-f triggers.sql  -f tests.sql

