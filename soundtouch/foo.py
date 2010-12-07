import os,re
RE_EXCLUDE = re.compile( r"(\.svn)" )
for root, dirs, files in os.walk("."):
    if RE_EXCLUDE.search( root ):
	continue
    for i in dirs:
	if RE_EXCLUDE.search( i ):
	    continue
	p = os.path.join( root, i )
	os.system( "svn add %s" % p )
    for i in files:
	if RE_EXCLUDE.search( i ):
	    continue
	p = os.path.join( root, i )
	os.system( "svn add %s" % p )
