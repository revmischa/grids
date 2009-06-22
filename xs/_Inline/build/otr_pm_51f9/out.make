/usr/bin/perl /usr/share/perl/5.10/ExtUtils/xsubpp  -typemap /usr/share/perl/5.10/ExtUtils/typemap   otr_pm_51f9.xs > otr_pm_51f9.xsc && mv otr_pm_51f9.xsc otr_pm_51f9.c
gcc -c  -I/home/lazarus/dev/grids/grids/xs -I/usr/include -D_REENTRANT -D_GNU_SOURCE -DDEBIAN -fno-strict-aliasing -pipe -I/usr/local/include -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -O2 -g   -DVERSION=\"0.00\" -DXS_VERSION=\"0.00\" -fPIC "-I/usr/lib/perl/5.10/CORE"   otr_pm_51f9.c
otr_pm_51f9.xs:109: warning: initialization from incompatible pointer type
otr_pm_51f9.xs: In function ‘otrInit’:
otr_pm_51f9.xs:136: warning: passing argument 3 of ‘Perl_sv_setiv’ makes integer from pointer without a cast
otr_pm_51f9.xs: In function ‘otrSend’:
otr_pm_51f9.xs:178: warning: initialization makes pointer from integer without a cast
otr_pm_51f9.xs: In function ‘otrReceive’:
otr_pm_51f9.xs:240: warning: initialization makes pointer from integer without a cast
otr_pm_51f9.xs: At top level:
otr_pm_51f9.xs:294: warning: conflicting types for ‘otrDisconnect’
otr_pm_51f9.xs:260: warning: previous implicit declaration of ‘otrDisconnect’ was here
otr_pm_51f9.xs:299: warning: conflicting types for ‘otrStartstop’
otr_pm_51f9.xs:290: warning: previous implicit declaration of ‘otrStartstop’ was here
otr_pm_51f9.xs: In function ‘otrStartstop’:
otr_pm_51f9.xs:305: warning: initialization makes pointer from integer without a cast
otr_pm_51f9.xs: In function ‘otrMessageDisconnect’:
otr_pm_51f9.xs:331: warning: initialization makes pointer from integer without a cast
otr_pm_51f9.xs: In function ‘otrGetContext’:
otr_pm_51f9.xs:351: warning: initialization makes pointer from integer without a cast
otr_pm_51f9.xs: In function ‘cb_create_privkey’:
otr_pm_51f9.xs:536: warning: initialization makes pointer from integer without a cast
otr_pm_51f9.xs: In function ‘cb_inject_message’:
otr_pm_51f9.xs:579: warning: passing argument 1 of ‘cHandleOtrInjectMessage’ discards qualifiers from pointer target type
otr_pm_51f9.xs:579: warning: passing argument 2 of ‘cHandleOtrInjectMessage’ discards qualifiers from pointer target type
otr_pm_51f9.xs:579: warning: passing argument 3 of ‘cHandleOtrInjectMessage’ discards qualifiers from pointer target type
otr_pm_51f9.xs:579: warning: passing argument 4 of ‘cHandleOtrInjectMessage’ discards qualifiers from pointer target type
otr_pm_51f9.xs: In function ‘cb_notify’:
otr_pm_51f9.xs:598: warning: passing argument 1 of ‘cOtrNotify’ discards qualifiers from pointer target type
otr_pm_51f9.xs:598: warning: passing argument 3 of ‘cOtrNotify’ discards qualifiers from pointer target type
otr_pm_51f9.xs:598: warning: passing argument 4 of ‘cOtrNotify’ discards qualifiers from pointer target type
otr_pm_51f9.xs: In function ‘cb_display_otr_message’:
otr_pm_51f9.xs:612: warning: passing argument 1 of ‘cHandleOtrMessage’ discards qualifiers from pointer target type
otr_pm_51f9.xs:612: warning: passing argument 2 of ‘cHandleOtrMessage’ discards qualifiers from pointer target type
otr_pm_51f9.xs:612: warning: passing argument 3 of ‘cHandleOtrMessage’ discards qualifiers from pointer target type
otr_pm_51f9.xs: In function ‘cb_new_fingerprint’:
otr_pm_51f9.xs:646: warning: passing argument 1 of ‘cHandleOtrNewFingerprint’ discards qualifiers from pointer target type
otr_pm_51f9.xs: In function ‘cb_write_fingerprints’:
otr_pm_51f9.xs:653: warning: initialization makes pointer from integer without a cast
Running Mkbootstrap for otr_pm_51f9 ()
chmod 644 otr_pm_51f9.bs
rm -f blib/arch/auto/otr_pm_51f9/otr_pm_51f9.so
cc  -shared -O2 -g -L/usr/local/lib otr_pm_51f9.o  -o blib/arch/auto/otr_pm_51f9/otr_pm_51f9.so 	\
	   -L/usr/lib -lotr  	\
	  
chmod 755 blib/arch/auto/otr_pm_51f9/otr_pm_51f9.so
cp otr_pm_51f9.bs blib/arch/auto/otr_pm_51f9/otr_pm_51f9.bs
chmod 644 blib/arch/auto/otr_pm_51f9/otr_pm_51f9.bs
