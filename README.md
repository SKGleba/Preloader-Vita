# DEX Activation and spoof patcher.

You only need vitasdk to build it.
Works on fw 3.60 and 3.61 , for higher fws you need to change offsets.

This code is a MOD of zecoxao's memdump-spoof-devkit.

IDFK why it works like this.
Basically if you spoof TOOL before DEX the activation changes to +3227 days.
My guess is that its somehow related to sceRtc & SceSblSsMgr.
So i wanted to make a light devkit spoofer to load before testkit spoof,but it always fully spoofed devkit, then i made some mistakes in the code, but it worked x).
If you add "mdmp();" it will be a normal, working spoofer.
You can change the spoofed model/device by editing the 4th value in dcd[].

Credits:
- Zecoxao - for memdump-spoof.
- YifanLu - As mentioned in original credits - for mmu_dump.
- Team Molecule - for henkaku, taihen, and enso.
- Anonymous - As mentioned in original credits - for precious info.
