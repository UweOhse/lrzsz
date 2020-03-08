/* zmodem.h - ZMODEM protocol constants

  Copyright (C) until 1998 Chuck Forsberg (OMEN Technology Inc)
  Copyright (C) 1996, 1997 Uwe Ohse

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
  02111-1307, USA.

 *    05-23-87  Chuck Forsberg Omen Technology Inc
*/

#define ZPAD       '*'	/* 052 Padding character begins frames */
#define ZDLE       030	/* Ctrl-X Zmodem escape - `ala BISYNC DLE */
#define ZDLEE   (ZDLE^0100)	/* Escaped ZDLE as transmitted */
#define ZBIN       'A'	/* Binary frame indicator */
#define ZHEX       'B'	/* HEX frame indicator */
#define ZBIN32     'C'	/* Binary frame with 32 bit crc */
#define ZBINR32    'D'	/* RLE packed binary frame with 32 bit crc */
#define ZVBIN      'a'  /* binary frame indicator (crc16) */
#define ZVHEX      'b'  /* HEX frame indicator */
#define ZVBIN32    'c'  /* Binary frame indicator with 32 bit crc */
#define ZVBINR32   'd'  /* RLE packed Binary frame indicator with 32 bit crc */
#define ZRESC     0x7e  /* RLE flag/escape character */
#define ZMAXHLEN   16   /* Max header information length */
#define ZMAXSPLEN 1024  /* Max subpacket length */

/* Frame types (see array "frametypes" in zm.c) */
#define ZRQINIT	     0	/* Request receive init */
#define ZRINIT	     1	/* Receive init */
#define ZSINIT       2	/* Send init sequence (optional) */
#define ZACK         3	/* ACK to above */
#define ZFILE        4	/* File name from sender */
#define ZSKIP        5	/* To sender: skip this file */
#define ZNAK         6	/* Last packet was garbled */
#define ZABORT       7	/* Abort batch transfers */
#define ZFIN         8	/* Finish session */
#define ZRPOS        9	/* Resume data trans at this position */
#define ZDATA       10	/* Data packet(s) follow */
#define ZEOF        11	/* End of file */
#define ZFERR       12	/* Fatal Read or Write error Detected */
#define ZCRC        13	/* Request for file CRC and response */
#define ZCHALLENGE  14	/* Receiver's Challenge */
#define ZCOMPL      15	/* Request is complete */
#define ZCAN        16	/* Other end canned session with CAN*5 */
#define ZFREECNT    17	/* Request for free bytes on filesystem */
#define ZCOMMAND    18	/* Command from sending program */
/* this seems to have vanished, wasn't found in later omen documents */
#define ZSTDERR     19	/* Output to standard error, data follows. */

/* ZDLE sequences */
#define ZCRCE 'h'	/* CRC next, frame ends, header packet follows */
#define ZCRCG 'i'	/* CRC next, frame continues nonstop */
#define ZCRCQ 'j'	/* CRC next, frame continues, ZACK expected */
#define ZCRCW 'k'	/* CRC next, ZACK expected, end of frame */
#define ZRUB0 'l'	/* Translate to rubout 0177 */
#define ZRUB1 'm'	/* Translate to rubout 0377 */

/* zdlread return values (internal) */
/* -1 is general error, -2 is timeout */
#define GOTOR 0400
#define GOTCRCE (ZCRCE|GOTOR)	/* ZDLE-ZCRCE received */
#define GOTCRCG (ZCRCG|GOTOR)	/* ZDLE-ZCRCG received */
#define GOTCRCQ (ZCRCQ|GOTOR)	/* ZDLE-ZCRCQ received */
#define GOTCRCW (ZCRCW|GOTOR)	/* ZDLE-ZCRCW received */
#define GOTCAN	(GOTOR|030)	/* CAN*5 seen */

/* Byte positions within header array */
#define ZF0	3	/* First flags byte */
#define ZF1	2
#define ZF2	1
#define ZF3	0
#define ZP0	0	/* Low order 8 bits of position */
#define ZP1	1
#define ZP2	2
#define ZP3	3	/* High order 8 bits of file position */

/* Parameters for ZRINIT header */
#define ZRPXWN	8	/* 9th byte in header contains window size/256 */
#define ZRPXQQ	9	/* 10th to 14th bytes contain quote mask */
/* Bit Masks for ZRINIT flags byte ZF0 */
#define CANFDX	0x01	/* Rx can send and receive true FDX */
#define CANOVIO	0x02	/* Rx can receive data during disk I/O */
#define CANBRK	0x04	/* Rx can send a break signal */
// the following once was defined, but later dropped, and the bit reused */
// #define CANCRY	0x08	/* Receiver can decrypt */
#define CANRLE	0x08	/* Receiver can decode RLE */
#define CANLZW	0x10	/* Receiver can uncompress */
#define CANFC32	0x20	/* Receiver can use 32 bit Frame Check */
#define ESCCTL  0x40	/* Receiver expects ctl chars to be escaped */
#define ESC8    0x80	/* Receiver expects 8th bit to be escaped */
/* Bit Masks for ZRINIT flags byze ZF1 */
#define ZF1_CANVHDR  0x01  /* Variable headers OK, unused in lrzsz */
#define ZF1_TIMESYNC 0x02 /* nonstandard, Receiver request timesync */
#define ZF1_ZRRQWN   0x08 /* receiver specified window size in ZRPXWN */
#define ZF1_ZRRQQQ   0x10 /* additional control chars to quote in ZRPXQQ */
#define ZF1_ZRQNVH   (ZF1_ZRRQQQ|ZF1_ZRRQWN) /* Variable len hdr reqd to access info */

/* Parameters for ZSINIT frame */
#define ZATTNLEN 32	/* Max length of attention string */
// why is this ZF1??? -- uwe
#define ALTCOFF  ZF1    /* Offset to alternate "canit" string, 0 if not used */
/* Bit Masks for ZSINIT flags byte ZF0 */
// fortunately identical to ESCCTL and ESC8.
#define TESCCTL 0x40	/* Transmitter expects ctl chars to be escaped */
#define TESC8   0x80	/* Transmitter expects 8th bit to be escaped */

/* Parameters for ZFILE frame */
/* Conversion options one of these in ZF0 */
#define ZCBIN	1	/* Binary transfer - inhibit conversion */
#define ZCNL	2	/* Convert NL to local end of line convention */
#define ZCRESUM	3	/* Resume interrupted file transfer */
/* Management include options, one of these ored in ZF1 */
#define ZF1_ZMSKNOLOC   0x80 /* Skip file if not present at rx */
/* Management options, one of these ored in ZF1 */
#define ZF1_ZMMASK 	0x1f /* Mask for the choices below */
#define ZF1_ZMNEWL         1 /* Transfer if source newer or longer */
#define ZF1_ZMCRC          2 /* Transfer if different file CRC or length */
#define ZF1_ZMAPND         3 /* Append contents to existing file (if any) */
#define ZF1_ZMCLOB         4 /* Replace existing file */
#define ZF1_ZMNEW          5 /* Transfer if source newer */
#define ZF1_ZMDIFF         6 /* Transfer if dates or lengths different */
#define ZF1_ZMPROT         7 /* Protect destination file */
#define ZF1_ZMCHNG         8 /* Change filename if destination exists */

/* Transport options, one of these in ZF2 */
#define ZTLZW	1	/* Lempel-Ziv compression */
// later dropped 
// #define ZTCRYPT	2	/* Encryption */
#define ZTRLE	3	/* Run Length encoding */
/* Extended options for ZF3, bit encoded */
#define ZXSPARS		0x40	/* Encoding for sparse file operations */
#define ZCANVHDR	0x01	/* Variable headers OK */
// Receiver window size override
// i thought that was done with ZRRQWN and ZRPXWN? -- uwe
#define ZRWOVR          0x04   /* byte position for receive window override/256 */

/* Parameters for ZCOMMAND frame ZF0 (otherwise 0) */
#define ZCACK1	1	/* Acknowledge, then do command */


/* End of ZMODEM.H */
