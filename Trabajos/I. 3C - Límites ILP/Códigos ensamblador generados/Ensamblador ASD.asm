// Problema A /Od /IA32
	for (i=0;i<N_ELEM;i++)
	{
		z = z + a[i];
002F1150  addss       xmm0,dword ptr [eax-4]  
002F1155  addss       xmm0,dword ptr [eax]  
002F1159  addss       xmm0,dword ptr [eax+4]  
002F115E  addss       xmm0,dword ptr [eax+8]  
002F1163  addss       xmm0,dword ptr [eax+0Ch]  
002F1168  addss       xmm0,dword ptr [eax+10h]  
002F116D  addss       xmm0,dword ptr [eax+14h]  
002F1172  addss       xmm0,dword ptr [eax+18h]  
002F1177  add         eax,20h  
002F117A  cmp         eax,2FF3C4h  
002F117F  jl          problemA+10h (02F1150h)  
002F1181  movss       dword ptr [ebp-4],xmm0  
	}

	return z;
002F1186  fld         dword ptr [z]  
}
002F1189  mov         esp,ebp  
002F118B  pop         ebp  
002F118C  ret 

// Problema A /Od /SSE
	for (i=0;i<N_ELEM;i++)
	{
		z = z + a[i];
008A1150  addss       xmm0,dword ptr [eax-4]  
008A1155  addss       xmm0,dword ptr [eax]  
008A1159  addss       xmm0,dword ptr [eax+4]  
008A115E  addss       xmm0,dword ptr [eax+8]  
008A1163  addss       xmm0,dword ptr [eax+0Ch]  
008A1168  addss       xmm0,dword ptr [eax+10h]  
008A116D  addss       xmm0,dword ptr [eax+14h]  
008A1172  addss       xmm0,dword ptr [eax+18h]  
008A1177  add         eax,20h  
008A117A  cmp         eax,8AF3C4h  
008A117F  jl          problemA+10h (08A1150h)  
008A1181  movss       dword ptr [ebp-4],xmm0  
	}

	return z;
008A1186  fld         dword ptr [z]  
}
008A1189  mov         esp,ebp  
008A118B  pop         ebp  
008A118C  ret  

// Problema A /O2 /IA32
	for (i=0;i<N_ELEM;i++)
	{
		z = z + a[i];
00DD1150  addss       xmm0,dword ptr [eax-4]  
00DD1155  addss       xmm0,dword ptr [eax]  
00DD1159  addss       xmm0,dword ptr [eax+4]  
00DD115E  addss       xmm0,dword ptr [eax+8]  
00DD1163  addss       xmm0,dword ptr [eax+0Ch]  
00DD1168  addss       xmm0,dword ptr [eax+10h]  
00DD116D  addss       xmm0,dword ptr [eax+14h]  
00DD1172  addss       xmm0,dword ptr [eax+18h]  
00DD1177  add         eax,20h  
00DD117A  cmp         eax,0DDF3C4h  
00DD117F  jl          problemA+10h (0DD1150h)  
00DD1181  movss       dword ptr [ebp-4],xmm0  
	}

	return z;
00DD1186  fld         dword ptr [z]  
}
00DD1189  mov         esp,ebp  
00DD118B  pop         ebp  
00DD118C  ret  

// Problema A /O2 /SSE
for (i=0;i<N_ELEM;i++)
	{
		z = z + a[i];
00691150  addss       xmm0,dword ptr [eax-4]  
00691155  addss       xmm0,dword ptr [eax]  
00691159  addss       xmm0,dword ptr [eax+4]  
0069115E  addss       xmm0,dword ptr [eax+8]  
00691163  addss       xmm0,dword ptr [eax+0Ch]  
00691168  addss       xmm0,dword ptr [eax+10h]  
0069116D  addss       xmm0,dword ptr [eax+14h]  
00691172  addss       xmm0,dword ptr [eax+18h]  
00691177  add         eax,20h  
0069117A  cmp         eax,69F3C4h  
0069117F  jl          problemA+10h (0691150h)  
00691181  movss       dword ptr [ebp-4],xmm0  
	}

	return z;
00691186  fld         dword ptr [z]  
}
00691189  mov         esp,ebp  
0069118B  pop         ebp  
0069118C  ret  

// Problema B /Od /SSE
007811A0  movss       xmm1,dword ptr b (07853C0h)[eax]  
007811A8  addss       xmm1,dword ptr a (078D3C0h)[eax]  
007811B0  addss       xmm1,xmm0  
007811B4  movss       xmm0,dword ptr [eax+78D3C4h]  
007811BC  addss       xmm0,dword ptr [eax+7853C4h]  
007811C4  addss       xmm1,xmm0  
007811C8  movss       xmm0,dword ptr [eax+78D3C8h]  
007811D0  addss       xmm0,dword ptr [eax+7853C8h]  
007811D8  addss       xmm1,xmm0  
007811DC  movss       xmm0,dword ptr [eax+78D3CCh]  
007811E4  addss       xmm0,dword ptr [eax+7853CCh]  
007811EC  addss       xmm1,xmm0  
007811F0  movss       xmm0,dword ptr [eax+78D3D0h]  
007811F8  addss       xmm0,dword ptr [eax+7853D0h]  
00781200  addss       xmm1,xmm0  
00781204  movss       xmm0,dword ptr [eax+78D3D4h]  
0078120C  addss       xmm0,dword ptr [eax+7853D4h]  
00781214  addss       xmm1,xmm0  
00781218  movss       xmm0,dword ptr [eax+78D3D8h]  
00781220  addss       xmm0,dword ptr [eax+7853D8h]  
00781228  addss       xmm1,xmm0  
0078122C  movss       xmm0,dword ptr [eax+78D3DCh]  
00781234  addss       xmm0,dword ptr [eax+7853DCh]  
0078123C  add         eax,20h  
0078123F  addss       xmm0,xmm1  
00781243  cmp         eax,2000h  
00781248  jl          problemB+10h (07811A0h)  
0078124E  movss       dword ptr [ebp-4],xmm0  

// Problema C /Od /SSE
00C91270  cmp         dword ptr cond3 (0C9F3C0h)[ecx],1  
00C91277  mov         eax,offset b (0C953C0h)  
00C9127C  cmove       eax,edx  
00C9127F  addss       xmm0,dword ptr [eax+ecx]  
00C91284  add         ecx,4  
00C91287  movss       dword ptr [ebp-4],xmm0  
00C9128C  cmp         ecx,2000h  
00C91292  jl          problemC+10h (0C91270h)  
00C91294  fld         dword ptr [ebp-4]  
	
// Problema D1 /Od /SSE
00B712B0  cmp         dword ptr cond1 (0B873C0h)[ecx],1  
00B712B7  mov         eax,offset b (0B753C0h)  
00B712BC  cmove       eax,edx  
00B712BF  addss       xmm0,dword ptr [eax+ecx]  
00B712C4  add         ecx,4  
00B712C7  movss       dword ptr [ebp-4],xmm0  
00B712CC  cmp         ecx,2000h  
00B712D2  jl          problemD1+10h (0B712B0h)  
00B712D4  fld         dword ptr [ebp-4] 	

// Problema D2 /Od /SSE
009612F0  cmp         dword ptr cond2 (09753C0h)[ecx],1  
009612F7  mov         eax,offset b (09653C0h)  
009612FC  cmove       eax,edx  
009612FF  addss       xmm0,dword ptr [eax+ecx]  
00961304  add         ecx,4  
00961307  movss       dword ptr [ebp-4],xmm0  
0096130C  cmp         ecx,2000h  
00961312  jl          problemD2+10h (09612F0h)  
00961314  fld         dword ptr [ebp-4] 

// Ejemplo /Od /SSE
00961330  movss       xmm0,dword ptr b (09653C0h)[eax]  
00961338  addss       xmm0,xmm1  
0096133C  movss       dword ptr a (096D3C0h)[eax],xmm0  
00961344  movss       xmm0,dword ptr [eax+9653C4h]  
0096134C  addss       xmm0,xmm1  
00961350  movss       dword ptr [eax+96D3C4h],xmm0  
00961358  movss       xmm0,dword ptr [eax+9653C8h]  
00961360  addss       xmm0,xmm1  
00961364  movss       dword ptr [eax+96D3C8h],xmm0  
0096136C  movss       xmm0,dword ptr [eax+9653CCh]  
00961374  addss       xmm0,xmm1  
00961378  movss       dword ptr [eax+96D3CCh],xmm0  
00961380  movss       xmm0,dword ptr [eax+9653D0h]  
00961388  addss       xmm0,xmm1  
0096138C  movss       dword ptr [eax+96D3D0h],xmm0  
00961394  movss       xmm0,dword ptr [eax+9653D4h]  
0096139C  addss       xmm0,xmm1  
009613A0  movss       dword ptr [eax+96D3D4h],xmm0  
009613A8  movss       xmm0,dword ptr [eax+9653D8h]  
009613B0  addss       xmm0,xmm1  
009613B4  movss       dword ptr [eax+96D3D8h],xmm0  
009613BC  movss       xmm0,dword ptr [eax+9653DCh]  
009613C4  addss       xmm0,xmm1  
009613C8  movss       dword ptr [eax+96D3DCh],xmm0  
009613D0  add         eax,20h  
009613D3  cmp         eax,2000h  
009613D8  jl          example+10h (0961330h)