
freertps���g�p����MCU�ւ�ROS2�����f��
====================================

�f���̊T�v
----------
���l�T�X��RX63N�����ڂ��ꂽArduino�݊��{�[�h�iGR-SAKURA II-FULL�j���g�p����ROS2�����������f���ł��B

�Q�l�F
ROSCon2017�ł̃��C�g�j���O�g�[�N�v���[��
https://roscon.ros.org/2017/presentations/ROSCon%202017%20Lightning%20110.pdf
https://vimeo.com/236129109

RX63N
https://www.renesas.com/ja-jp/products/microcontrollers-microprocessors/rx.html

GR-SAKURA II-FULL
http://gadget.renesas.com/ja/product/sakura.html


�����ł́AGR-SAKURA II-FULL��œ��삷��freertps���g���āA�f�������s����菇�������܂��B

* Hello world�f��
  ROS2�̕�����p���b�Z�[�W�^"std_msgs::msg::String"�̑���M�f��

* Tilt Control�f��
  ROS2��Joystick�p���b�Z�[�W�^"sensor_msgs::msg::Joy"���g����Joystick�ɂ��p���`���g��̐���f��


�f�����s��
------------

�f���̎��s�ɕK�v�ȍ��ڂ͈ȉ��̂Ƃ���ł��B

### **\[�n�[�h�E�F�A\]**

(1) GR-SAKURA II-FULL 2��
�쐬�����C���[�W�����삷��^�[�Q�b�g�{�[�h�B2����GR-SAKURA II-FULL�i�ȍ~�ASAKURA�j���K�v�ł��B�ȉ��ōw�����\�ł��B

http://wakamatsu.mobi/eccube/html/products/detail.php?product_id=12010659
http://jp.rs-online.com/web/p/processor-microcontroller-development-kits/1229826/

* SAKURA(Pub): ���b�Z�[�W�𑗐M����{�[�h�BJoystick�V�[���h�����t���Ďg�p���܂��B
* SAKURA(Sub): ���b�Z�[�W����M����{�[�h�B�p���`���g���UART�ڑ����Ďg�p���܂��B

![GR-SAKURA](docs/renesas_rx/images/GR-SAKURA.png)

(2) Joystick�V�[���h
SAKURA(Pub)�ɐڑ�����Arduino�V�[���h�ł��B�ȉ��Ȃǂōw�����\�ł��B
https://www.sainsmart.com/products/sainsmart-joystick-shied-expansion-board-for-arduino

���̃V�[���h��SAKURA�ɒ��ړ��ڂ����RJ45�|�[�g�Ɗ����܂��B�s���w�b�_�[�Ȃǂŉ�����Ă��������B

(3) �p���`���g��
���̃A�v���̂��߂ɍ쐬�����p���`���g��ł��i�s�̕i�ł͂���܂���j�B�p���`���g�䐧��A�v���i��q�j��Joystick�̈ʒu�������[�^�[�̖ڕW�p�x�ɕϊ����A����ɏ]���p���`���g�䂪���삵�܂��B

(4) Ethernet HUB
SAKURA�Ԃ�LAN�ڑ����邽�߂ɕK�v�ł��B

(5) E1 Emulator (�I�v�V����)
RX63N�Ŏg�p�\��ICE�f�o�b�K�ł��B
Hello world�f���ɂ����āA��M���̃f�o�b�O�E�R���\�[�����o�͂���ꍇ�ɕK�v�ƂȂ�܂��B

���̃f���ł́A��L(1)�`(5)�̋@��͈ȉ��̂悤�ɐڑ�����܂��B

```
 +---------------------+
 |    Joystick         |
 +---------------------+
         | Arduino Shield I/F
 +---------------------+           +-----------------+
 |   SAKURA(Pub)       +-----------+  Ethernet HUB   |
 +---------------------+  Ethernet +-------+---------+
   192.168.1.100                           |
                                           |
   192.168.1.101                           |
 +---------------------+                   |
 |   SAKURA(Sub)       +-------------------+
 +---------------------+  Ethernet
         | UART                                
 +---------------------+
 |  �p���`���g��        |
 +---------------------+
```

### **\[�\�t�g�E�F�A\]**

(1) C/C++ Compiler Package for RX Family

�����]���ł��ȉ�����_�����[�h���܂��B�C���X�g�[�����@�ɂ��Ă�Web�y�[�W��̃h�L�������g���Q�Ƃ��Ă��������B

https://www.renesas.com/ja-jp/software/D4000427.html

�Ȃ��A�����]���łɂ͋@�\�A�����N�T�C�Y�ɐ���������܂��̂�web�ł̊m�F�����肢���܂��B
����m�F���s�����o�[�W�����́uV.6.00.00�v�ł��B

(2) Flash Development Toolkit 

�����]���ł��ȉ�����_�����[�h���܂��B�C���X�g�[�����@�ɂ��Ă�Web�y�[�W��̃h�L�������g���Q�Ƃ��Ă��������B

https://www.renesas.com/ja-jp/software/D3017333.html

����m�F���s�����o�[�W�����́uV.4.09 Release 03�v�ł��B

�\�[�X�R�[�h�̎擾�ƓW�J
------------

(1) �ȉ��̃\�[�X�R�[�h����肵�A�C�ӂ̃f�B���N�g�ɓW�J���܂��B

RX�t�@�~�� �g�ݍ��ݗpTCP/IP M3S-T4-Tiny��p�����T���v���v���O���� Firmware Integration Technology
https://www.renesas.com/ja-jp/software/D6000791.html

(2) �W�J�����f�B���N�g�����̈ȉ��̃t�@�C����ҏW���܂��B

```
workspace/sample/rx63n_gr_sakura/r_config/r_bsp_config.h
```

UART�ւ̋����N���b�N��ύX���邽�߁A�ȉ��̃}�N�� BSP_CFG_PCKB_DIV�̒l��(4)����(2)�ɕύX���܂��B����͍���g�p����p���`���g��Ƃ�UART�ʐM�łPMbps�̐ݒ���g�p���邽�߂̂��̂ŁAUART�ʐM�̎d�l�ɍ��킹�ĕύX���܂��B


```c
    /* Peripheral Module Clock B Divider (PCKB).
      Available divisors = /1 (no division), /2, /4, /8, /16, /32, /64
    */
    #define BSP_CFG_PCKB_DIV                (2)
```


(3) �ȉ���URL���RX63N�ł�freertps����肵�܂��B
https://github.com/godzilla-max/freertps/archive/porting-to-rx.zip
��L�A�[�J�C�u���̃f�B���N�g�������ufreertps�v�ɕύX���āA�ȉ��̃f�B���N�g�����œW�J���Ă��������B

```
    workspace/sample/rx63n_gr_sakura/
```

(4) �ȉ���URL���f���A�v���P�[�V�����Ŏg�p����UART�h���C�o����肵�܂��B
https://github.com/godzilla-max/uart/archive/master.zip
��L�A�[�J�C�u�͈ȉ��̃f�B���N�g�����œW�J���Ă��������B

```
    workspace/sample/rx63n_gr_sakura/
```


(1)�`(4)�̎菇�ō쐬�����f�B���N�g���\���͈ȉ��̂Ƃ���ł��B

```
    workspace/sample/rx63n_gr_sakura/ [TCP/IP M3S-T4-Tiny ��p�����T���v���v���O����]
    |
    |-- freertps/                     [RX63N�� freertps�̃R�[�h�ꎮ]
    |   |
    |   |-- apps/ [FreeRTPS�𗘗p����A�v���̃T���v��]
    |   |   |-- ...(SNIPPED)...
    |   |   |-- rx63n_gr_sakura_joystick/ [Joystick����A�v��]
    |   |   |-- rx63n_gr_sakura_listener/ [������"Hello world"�̎�M�A�v��]
    |   |   |-- rx63n_gr_sakura_talker/   [������"Hello world"�̑��M�A�v��]
    |   |   |-- rx63n_gr_sakura_tilt_ctrl/[�p���`���g�䐧��A�v��]
    |   |   |-- ...(SNIPPED)...
    |   |
    |   |-- include/
    |   |-- psm/
    |   |-- r2/
    |   |-- ros2_demos/
    |   |-- rosmsg_apps/
    |   |-- scripts/
    |   |-- systems/
    |   |   |-- ...(SNIPPED)...
    |   |   |-- rx63n_gr_sakura/      [freertps RX63N + M3S-T4-Tiny�ˑ�����]
    |   |   |-- ...(SNIPPED)...
    |   |-- tests/
    |   |-- tiny/
    |   `-- utils/
    |-- HardwareDebug/
    |-- r_bsp/
    |-- r_cmt_rx/
    |-- r_config/
    |-- r_ether_rx/
    |-- r_pincfg/
    |-- r_sys_time_rx/
    |-- r_t4_driver_rx/
    |-- r_t4_rx/
    |-- src/
    `-- uart/                         [UART�h���C�o]
```

�t�@�[���E�F�A�̃r���h���@
------------

(1) SAKURA(RX63N�j�p�Ƀr���h���s���܂��B

�ڍׂ͈ȉ��ɋL�ڂ���Ă��܂����ARX63N�p��CS+�v���W�F�N�g�ɕϊ����܂��B
RX �t�@�~�� �g�ݍ��ݗpTCP/IP M3S-T4-Tiny ��p�����T���v���v���O�����i��T�́j
https://www.renesas.com/ja-jp/doc/products/mpumcu/apn/rx/002/r20an0312jj0106-rx-t4.pdf

RX63N�v���W�F�N�g�ϊ��ł́A�ȉ��̂悤�ɐݒ肵�܂��B�}�C�N���R���g���[���̕i�Ԃ��Ⴄ���ƁA���߃Z�b�g�E�A�[�L�e�N�`���̐ݒ��ύX���Ȃ����Ƃɒ��ӂ��Ă��������B

(1-1) CS+ for CC ���N�����A�ue2 studio / CubeSuite / �c�v�́uGO�v�{�^���������܂��B

![import_step1_1](docs/renesas_rx/images/csplus_import2.png)

����ȍ~��CS+ for CC���N������邱�ƂŁA��L�̉�ʂ��o�͂���Ȃ��ꍇ�́A��ʓ��̈ȉ��̃{�^�����N���b�N���Ă��������B

![import_step1_2](docs/renesas_rx/images/csplus_import1.png)

(1-2) �u�v���W�F�N�g���J���v�̉�ʓ��ŁA�ȉ��̂悤�Ɂurx63n_gr_sakura.rcpc�v��I�����Ă��������B
    
![import_step2_3](docs/renesas_rx/images/csplus_import3.png)
    

(1-3) �u�v���W�F�N�g�ϊ��ݒ�v�E�C���h�E���J���A�c���[��Ńv���W�F�N�g��I�����܂��B

![import_step3](docs/renesas_rx/images/csplus_import4.png)

(1-4) �c���[�̉E���̃v���W�F�N�g�ݒ�ŁA�g�p����}�C�N���R���g���[�����uRX63N�v->�uR5F563NYDxFP�v��I�����āuOK�v�������܂��BCS+�͕ϊ����ꂽ�v���W�F�N�g���o�͂��܂��B

![import_step3](docs/renesas_rx/images/csplus_import5.png)

�ȉ��̂悤�ȃ_�C�A���O���o�͂��ꂽ�ꍇ�́A�u�������v���N���b�N���Ă�������

![import_step3](docs/renesas_rx/images/csplus_import6.png)

(1-5)�u�v���W�F�N�g�c���[�v����uCC-RX�v��I�����܂��B

![import_step3](docs/renesas_rx/images/csplus_import7.png)

(1-6)�u���ʃI�v�V�����v�^�u->�uCPU�v->�u���߃Z�b�g�E�A�[�L�e�N�`���v�͕ύX���܂���B

![import_step3](docs/renesas_rx/images/csplus_import8.png)

(1-7)�u���ʃI�v�V�����v�^�u->�u�悭�g���I�v�V����(�R���p�C��)�v->�u�œK�����x���v��"0"�ɐݒ肵�Ă��������B

![import_step3](docs/renesas_rx/images/csplus_import9.png)

(2) src/�ȉ��̃t�@�C�������ׂăr���h�ΏۊO�Ƃ��邽�߁A�u�v���W�F�N�g�c���[�v�́u�t�@�C���v->�usrc�v��Łu�E�N���b�N�v->�u�v���p�e�B�v��I����A�u�r���h�̑ΏۂƂ���v���u�������v�ɐݒ肵�܂��B

![build_step2](docs/renesas_rx/images/csplus1.png)

(3) �ȉ��̃f�B���N�g�����u�t�@�C���v��Ƀh���b�O���h���b�v���Ă��������B

```
workspace/sample/rx63n_gr_sakura/uart
```

![build_step3](docs/renesas_rx/images/csplus2.png)

�u�t�H���_�ƃt�@�C���ǉ��v�̉�ʂł́A�u��������T�u�t�H���_�̊K�w���v�ɏ\���[���K�w���Ƃ���"10"��ݒ肵�āA�uOK�v���N���b�N���Ă��������B

![build_step5](docs/renesas_rx/images/csplus4_2.png)

(4) �u�t�@�C���v��Łu�E�N���b�N�v->�u�V�����J�e�S����ǉ��v��I����A�ufreertps�v�Ɠ��͂��Ă��������B

![build_step4](docs/renesas_rx/images/csplus3.png)

(5) �ȉ��̃f�B���N�g���E�t�@�C�����u�t�@�C���v->�ufreertps�v��Ƀh���b�O���h���b�v���Ă��������B

```
workspace/sample/rx63n_gr_sakura/freertps/apps
workspace/sample/rx63n_gr_sakura/freertps/include
workspace/sample/rx63n_gr_sakura/freertps/systems
workspace/sample/rx63n_gr_sakura/freertps/*.c
```

![build_step5](docs/renesas_rx/images/csplus4.png)

(6) �u�t�@�C���v->�ufreertps�v->�uapps�v���̈ȉ��̃f�B���N�g�����������ׂẴf�B���N�g���ɑ΂��āA�u�E�N���b�N�v->�u�v���W�F�N�g����O���v�����s���Ă��������B

```
rx63n_gr_sakura_joystick
rx63n_gr_sakura_listener
rx63n_gr_sakura_talker
rx63n_gr_sakura_tilt_ctrl
```

![build_step6](docs/renesas_rx/images/csplus5.png)

(7) �u�t�@�C���v->�ufreertps�v->�usystems�v���̈ȉ��̃f�B���N�g�����������ׂẴf�B���N�g���ɑ΂��āA�u�E�N���b�N�v->�u�v���W�F�N�g����O���v�����s���Ă��������B

```
rx63n_gr_sakura
```

![build_step7](docs/renesas_rx/images/csplus6.png)

(8) �u�t�@�C���v->�ufreertps�v->�uapps�v�̈ȉ��̃f�B���N�g������r���h������A�v����I�����܂��B�r���h�Ώۂ̃A�v���ɑ΂��Ă̂݁A�f�B���N�g����Łu�E�N���b�N�v->�u�v���p�e�B�v��I����A�u�r���h�̑ΏۂƂ���v���u�͂��v�ɐݒ肵�Ă��������B����ȊO�̃f�B���N�g���ł́A�u�������v��ݒ肵�Ă��������B

```
rx63n_gr_sakura_joystick [Joystick����A�v��]
rx63n_gr_sakura_listener [������"Hello world"�̎�M�A�v��]
rx63n_gr_sakura_talker   [������"Hello world"�̑��M�A�v��]
rx63n_gr_sakura_tilt_ctrl[�p���`���g�䐧��A�v��]
```

���ȉ��̗�́u������"Hello world"�̑��M�A�v���v���r���h�Ώۂ����ꍇ�̗�ł��B

![build_step8](docs/renesas_rx/images/csplus7.png)

(9) �uCC-RX(�r���h�E�c�[��)�v->�u���ʃI�v�V�����v�̃E�B���h�E���Łu�ǉ��̃C���N���[�h�E�p�X�v�Ɉȉ���ǉ����Ă��������B

```
%ProjectFolder%/../%ProjectName%/freertps/include
```

![build_step9](docs/renesas_rx/images/csplus8.png)

(10) ���j���[�o�[����u�r���h�v->�u�r���h�E�v���W�F�N�g�v���N���b�N���Ă��������B

![build_step10](docs/renesas_rx/images/csplus9.png)

�ȉ����u�o�́v���ɏo�͂����΃t�@�[���E�F�A�̃r���h�͐����ł��B�x���͖������Ă��������B

```
------ �r���h�I��(�G���[:0��, �x��:XX��)(rx63n_gr_sakura, HardwareDebug) ------  
```

�t�@�[���E�F�A�̏������ݕ��@
------------

�t�@�[���E�F�A�̏������݂͈ȉ���2�ʂ�ōs�����Ƃ��ł��܂��B

* USB�o�R�ł̏�������
* E1 Emulator�o�R�ł̏�������

�������ݎ菇�͈ȉ��ł��B

### **\[USB�o�R�ł̏�������\]**

(1) SAKURA�̃X�C�b�` SW3���uRUN�v�̔��Α��ɃX���C�h�����܂��B

![flash_wr1](docs/renesas_rx/images/slide_sw3.png)

(2) SAKURA��USB�o�R��PC�Ɛڑ����܂��B

(3) SAKURA��RESET SW�iSW1�F�Ԃ��{�^���j�������܂��B

![flash_wr2](docs/renesas_rx/images/reset_sw.png)

(4) Flash Development Toolkit���N�����܂��B

(5) �u�悤�����I�v�̉�ʂŁA�u�V�K�v���W�F�N�g���[�N�X�y�[�X�̍쐬�v�Ƀ`�F�b�N��������ɁA�uOK�v���N���b�N���܂��B

![flash_wr5](docs/renesas_rx/images/fdt1.png)

(6) �u�V�K�v���W�F�N�g���[�N�X�y�[�X�v�̉�ʂŁA�u���[�N�X�y�[�X���v����͂������(��: Test)�A�uOK�v���N���b�N���܂��B

![flash_wr6](docs/renesas_rx/images/fdt2.png)

(7) �u�f�o�C�X�ƃJ�[�l���̑I���v�̉�ʂŁA�uGeneric BOOT Device�v��I��������Ɂu���ցv���N���b�N���܂��B

![flash_wr7](docs/renesas_rx/images/fdt3.png)

(8) �u�ʐM�|�[�g�v�̉�ʂŁA�uSelect Port�v����uUSB Direct�v��I��������Ɂu���ցv���N���b�N���܂��B

![flash_wr8](docs/renesas_rx/images/fdt4.png)

(9) �uFLASH Development Toolkit�v�̉�ʂŁA�ȉ��̂悤�ȃ_�C�A���O���o�͂��ꂽ�ꍇ�́A�uOK�v���N���b�N���܂��B

![flash_wr9](docs/renesas_rx/images/fdt5.png)

(10) �uSelect Device�v�̉�ʂŁA�uOK�v���N���b�N���܂��B

![flash_wr10](docs/renesas_rx/images/fdt7.png)

(11) �u�ėp�f�o�C�X�̊m�F�v�̉�ʂŁA�uOK�v���N���b�N���܂��B

![flash_wr11](docs/renesas_rx/images/fdt8.png)

(12) �u�f�o�C�X�ݒ�v�̉�ʂŁA�u���̓N���b�N�v�Ɂu12.00�v��I�����āA�u���ցv���N���b�N���܂��B

![flash_wr12](docs/renesas_rx/images/fdt9.png)

(13) �u�������݃I�v�V�����v�̊e�ݒ荀�ڂ����ꂼ��ȉ��̂悤�ɂȂ��Ă��邱�Ƃ��m�F���āA�u�����v���N���b�N���܂��B

* Protection => Automatic
* Messaging => Advanced
* Readback Verification => No

![flash_wr13](docs/renesas_rx/images/fdt10.png)

(14) ���j���[�u�t�@�C���v���N���b�N���A�u�f�[�^�t�@�C�����J���v���N���b�N���܂��B���̌�A�u�f�[�^�t�@�C���̃��[�h�v�̉�ʂŁA�urx63n_gr_sakura�v��I�����āA�u�J���v���N���b�N���Ă��������B

![flash_wr14](docs/renesas_rx/images/fdt11.png)

(15) ���j���[�u�f�o�C�X�v���N���b�N���A�u�Ώۃt�@�C���̃_�E�����[�h�v���N���b�N���Ă��������B

![flash_wr15](docs/renesas_rx/images/fdt12.png)

�ȉ��̏o�͂����Ώ������݊����ł��B

```
�������݂��������܂���
```

(16) Flash Development Toolkit���I�����܂��B

(17) SAKURA�̃X�C�b�` SW3���uRUN�v���ɖ߂��܂��B



### **\[E1 Emulator�o�R�ł̏�������\]**
(1) CS+�̃��j���[�́u�f�o�b�O�v->�u�g�p����f�o�b�O�E�c�[���v������uRX E1(JTAG)�v���N���b�N���܂��B

![csplus_wr1](docs/renesas_rx/images/csplus10.png)

(2) CS+�̃��j���[�́u�f�o�b�O�v->�u�f�o�b�O�E�c�[���փ_�E�����[�h�v���N���b�N���܂��B

![csplus_wr2](docs/renesas_rx/images/csplus11.png)

�ȉ��̃_�C�A���O���o�ď������݂Ɏ��s����ꍇ�́A�uRX E1(JTAG)�v�́u�ڑ��p�ݒ�v�^�u���́u�N���b�N�v->�u���C���E�N���b�N�E�\�[�X�v���uHOCO�v�ɐݒ肵�āA(2)�̎菇���Ď��{���Ă��������B

```
�_�E�����[�h�Ɏ��s���܂����B

[�G���[�̒��ڌ���]
���C���E�N���b�N���g���̒l�����������͂���Ă��܂���B(E1891713)
```

����m�F
----

### Hello world�f��

SAKURA�ԂŁA������p���b�Z�[�W�^"std_msgs::msg::String"�̓]�����ł��邱�Ƃ��m�F���܂��B
����m�F�ɂ͎�M���̃f�o�b�O�E�R���\�[���o�͂��m�F���邽�߂́AE1 Emulator���g�����f�o�b�O�R���\�[���̋@�\���K�v�ł��B

(1) ������"Hello world"�̑��M�A�v���݂̂�I�����ăr���h���s���ASAKURA(Pub)�ɏ������݂܂��B

(2) ������"Hello world"�̎�M�A�v���݂̂�I�����ăr���h���s���ASAKURA(Sub)�ɏ������݂܂��B

(3) SAKURA(Pub)�ɓd����ڑ����܂��B

(4) E1 Emulator��ڑ�������ԂŁASAKURA(Sub)�ɓd����ڑ����܂��B

(5) CS+�̃��j���[�́u�f�o�b�O�v->�u�f�o�b�O�E�c�[���֐ڑ��v���N���b�N���āASAKURA(Sub)���f�o�b�K�ɐڑ����܂��B

(6) CS+�̃��j���[�́u�f�o�b�O�v->�uCPU���Z�b�g�v���N���b�N���A�����āu�f�o�b�O�v->�u���s�v���N���b�N���āASAKURA(Sub)���N�������܂��B

(7) �ȉ��̂悤�ɏo�͂����ƁASAKURA�Ԃŕ�����]�����������Ă��܂��B

```
hello, world!
freertps INFO : udp init()
freertps INFO : ^^>>>user_cb<<< ch:0,eventID = ETHER_EV_LINK_ON
freertps INFO : frudp_generic_init()
freertps INFO : frudp_part_create() on domain_id 0
freertps INFO : frudp_init_participant_id()
freertps INFO : add ucast rx port 7410
....(SNIPPED)....
I heard: [Hello World: 4]
I heard: [Hello World: 5]
I heard: [Hello World: 6]
I heard: [Hello World: 7]
....(SNIPPED)....
```

�Ȃ��ASAKURA(Pub)��SAKURA(Sub)�́APC Linux���ROS2(Beta2�ȍ~)�t���f��talker��listener�ɑ�։\�ł��B��ւ���ꍇ�͈ȉ��̃R�}���h��Linux terminal�Ŏ��s���Ă��������B


\[SAKURA(Pub)�̑��\]

```
$ cd <ros2���r���h�������[�N�X�y�[�X>
$ . install/local_setup.bash
$ ros2 run demo_nodes_cpp  talker
```

\[SAKURA(Sub)�̑��\]

```
$ cd <ros2���r���h�������[�N�X�y�[�X>
$ . install/local_setup.bash
$ ros2 run demo_nodes_cpp listener_best_effort
```

### Tilt Control�f��

SAKURA(Pub)��ROS2��Joystick�p���b�Z�[�W�^"sensor_msgs::msg::Joy"�𑗐M���A������󂯎����SAKURA(Sub)���p���`���g��𐧌�ł��邱�Ƃ��m�F���܂��B

(1) Joystick����A�v���݂̂�I�����ăr���h���s���ASAKURA(Pub)�ɏ������݂܂��B

(2) �p���`���g�䐧��A�v���݂̂�I�����ăr���h���s���ASAKURA(Sub)�ɏ������݂܂��B

(3) SAKURA(Pub)�ASAKURA(Sub)�ɓd����ڑ����܂��B

(4) Joystick�𑀍삵�āA�p���`���g�䂪�A�����ē����Ă��邱�Ƃ��m�F���܂��B


�p���`���g��ɂ���
====

���̃A�v���̂��߂ɍ쐬�����p���`���g��̂��ߍw�����邱�Ƃ͂ł��܂���B
�p���`���g�䐧��A�v����Joystick�̈ʒu�������[�^�[�̖ڕW�p�x�ɕϊ����܂��B�ڕW�ʒu��SAKURA�iSub)��UART�ő��M���A�p���`���g�������RS485�M���ɕϊ����A�p���`���g��̃T�[�{���[�^�[�𐧌䂵�܂��B
SAKURA�iSub)����p���`���g��ւ�UART�ʐM�̐ڑ��d�l���ȉ��Ɏ����܂��B���̂悤�ɒʐM����Ă���ΐ��������삵�Ă��邱�ƂɂȂ�܂��B


UART�d�l (Serial channel-1, P20/TXD0, P21/RXD0)
Baud rate: 1M bps
Data format: 8-b data, 1-b stop bit, no parity, no flow control

����R�}���h�d�l (send only)
Byte
1st header 0xFF (fixed)
2nd header 0xFF (fixed)
3rd motor ID (0x01:x 0x02:y)
4th 0x1E (fixed) 
5th �ڕW�p�x�ilowest byte�j
6th �ڕW�p�x�ihighest byte�j

�ڕW�p�x��2048�i�����j+/-218 �͈̔͂Ŏw�肳��܂��B

�g�p���[�^�[
Dynamixel MX-64AR
http://support.robotis.com/en/product/actuator/dynamixel/mx_series/mx-64at_ar.htm


�A��
===
ROS Discourse�ɃX���b�h�𗧂Ă�\��ł��B


��������
====

* freertps��API frudp_listen()�̈����ł��鎞�ԊԊu�̐��x��1�b�P�ʂł��B1�b���Z�����ԊԊu���K�v�ȏꍇ��"0.0f"��ݒ肵�Ă��������B
* PC���ROS2�ւ̐ڑ��m�F�́A�~�h���E�F�A�Ƃ���Fast-RTPS���g���Ă�����ł̂ݍs���Ă��܂��B���̃~�h���E�F�A�ւ̐ڑ��͖��m�F�ł��B


