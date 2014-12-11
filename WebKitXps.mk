
WebKitXps.dll: dlldata.obj WebKitX_p.obj WebKitX_i.obj
	link /dll /out:WebKitXps.dll /def:WebKitXps.def /entry:DllMain dlldata.obj WebKitX_p.obj WebKitX_i.obj \
		kernel32.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
.c.obj:
	cl /c /Ox /DREGISTER_PROXY_DLL \
		$<

clean:
	@del WebKitXps.dll
	@del WebKitXps.lib
	@del WebKitXps.exp
	@del dlldata.obj
	@del WebKitX_p.obj
	@del WebKitX_i.obj
