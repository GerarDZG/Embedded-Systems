// SSP Basic SPI functions
// Date: 9/1/2012

#ifndef __SSP_H__
#define __SSP_H__

/* There are there modes in SSP: loopback, master or slave. */
/* Here are the combination of all the tests.
(1) LOOPBACK test:		LOOPBACK_MODE=1, TX_RX_ONLY=0, USE_CS=1;
(2) Serial EEPROM test:	LOOPBACK_MODE=0, TX_RX_ONLY=0, USE_CS=0; (default)
(3) TX(Master) Only:	LOOPBACK_MODE=0, SSP_SLAVE=0, TX_RX_ONLY=1, USE_CS=1;
(4) RX(Slave) Only:		LOOPBACK_MODE=0, SSP_SLAVE=1, TX_RX_ONLY=0, USE_CS=1 */

#define LOOPBACK_MODE	0		/* 1 is loopback, 0 is normal operation. */
#define SSP_SLAVE		0		/* 1 is SLAVE mode, 0 is master mode */
#define TX_RX_ONLY		0		/* 1 is TX or RX only depending on SSP_SLAVE
								flag, 0 is either loopback mode or communicate
								with a serial EEPROM. */

/* SPI read and write buffer size */
#define SSP_BUFSIZE		16
#define FIFOSIZE		8

#define DELAY_COUNT		10
#define MAX_TIMEOUT		0xFF

/* Port0.2 is the SSP select pin */
#define SSP0_SEL		(1 << 2)

/* SSP Status register */
#define SSPSR_TFE		(1 << 0)
#define SSPSR_TNF		(1 << 1)
#define SSPSR_RNE		(1 << 2)
#define SSPSR_RFF		(1 << 3)
#define SSPSR_BSY		(1 << 4)

/* SSP CR0 register */
#define SSPCR0_DSS		(1 << 0)
#define SSPCR0_FRF		(1 << 4)
#define SSPCR0_SPO		(1 << 6)
#define SSPCR0_SPH		(1 << 7)
#define SSPCR0_SCR		(1 << 8)

/* SSP CR1 register */
#define SSPCR1_LBM		(1 << 0)
#define SSPCR1_SSE		(1 << 1)
#define SSPCR1_MS		(1 << 2)
#define SSPCR1_SOD		(1 << 3)

/* SSP Interrupt Mask Set/Clear register */
#define SSPIMSC_RORIM	(1 << 0)
#define SSPIMSC_RTIM	(1 << 1)
#define SSPIMSC_RXIM	(1 << 2)
#define SSPIMSC_TXIM	(1 << 3)

/* SSP0 Interrupt Status register */
#define SSPRIS_RORRIS	(1 << 0)
#define SSPRIS_RTRIS	(1 << 1)
#define SSPRIS_RXRIS	(1 << 2)
#define SSPRIS_TXRIS	(1 << 3)

/* SSP0 Masked Interrupt register */
#define SSPMIS_RORMIS	(1 << 0)
#define SSPMIS_RTMIS	(1 << 1)
#define SSPMIS_RXMIS	(1 << 2)
#define SSPMIS_TXMIS	(1 << 3)

/* SSP0 Interrupt clear register */
#define SSPICR_RORIC	(1 << 0)
#define SSPICR_RTIC		(1 << 1)

/* ATMEL SEEPROM command set */
#define WREN		0x06		/* MSB A8 is set to 0, simplifying test */
#define WRDI		0x04
#define RDSR		0x05
#define WRSR		0x01
#define READ		0x03
#define WRITE		0x02

/* RDSR status bit definition */
#define RDSR_RDY	0x01
#define RDSR_WEN	0x02

extern void SSP0Init( void );
extern uint8_t SSP0Send(uint8_t buf);
extern void SSP1Init( void );
extern void SSP1Send(uint8_t Buf);

#endif  /* __SSP_H__ */
/*****************************************************************************
**                            End Of File
******************************************************************************/

