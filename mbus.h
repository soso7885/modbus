#ifndef MBUS_H
#define MBUS_H

#define TCPMBUSPROTOCOL			0
#define TCPQUERYMSGLEN			6
#define TCPRESPSETSIGNALLEN		6
#define TCPRESPEXCPFRMLEN		9
#define TCPRESPEXCPMSGLEN		3
#define TCPSENDQUERYLEN			12

#define READCOILSTATUS 			0x01
#define READINPUTSTATUS 		0x02
#define READHOLDINGREGS 		0x03
#define READINPUTREGS 			0x04
#define FORCESIGLEREGS 			0x05
#define PRESETEXCPSTATUS 		0x06
#define FORCEMUILTCOILS			0x15
#define PRESETMUILTREGS			0x16
#define EXCPTIONCODE			0x80
#define EXCPILLGFUNC			0x01
#define EXCPILLGDATAADDR		0x02
#define EXCPILLGDATAVAL			0x03
#define READCOILSTATUS_EXCP		129
#define READINPUTSTATUS_EXCP	130
#define READHOLDINGREGS_EXCP	131
#define READINPUTREGS_EXCP		132
#define FORCESIGLEREGS_EXCP		133
#define PRESETEXCPSTATUS_EXCP	134

#define FRMLEN 260  /* | 1byte | 1byte | 0~255byte | 2byte | */

static inline int carry(int bdiv, int div)
{
	int tmp;
	int ans;
	
	ans = bdiv / div;
	tmp = bdiv % div;
	if(tmp > 0){
		ans += 1;
	}

	return ans;
}

/* mudbus serila frame */
struct frm_para {
	unsigned int slvID;
	unsigned int len;
	unsigned char fc;
	unsigned int straddr;
	unsigned int act;			// The status to write (in FC 0x05/0x06) 
};
	
/* save modbus TCP parameter */
struct tcp_frm_para {
	unsigned short transID;
	unsigned short potoID;
	unsigned short msglen;
	unsigned char unitID;
	unsigned char fc;
	unsigned short straddr;
	unsigned short act;
	unsigned short len;
};

/* modbus TCP Query & fc = 05/06 frame */
struct tcp_frm{
	unsigned short transID;
	unsigned short potoID;
	unsigned short msglen;
	unsigned char unitID;
	unsigned char fc;
	unsigned short straddr;
	unsigned short act;
}__attribute__((packed));

/* modbus TCP respond excption frame */
struct tcp_frm_excp {
	unsigned short transID;
	unsigned short potoID;
	unsigned short msglen;
	unsigned char unitID;
	unsigned char fc;
	unsigned char ec;
}__attribute__((packed));
	
/* modbus TCP respond fc = 01/02/03/04 frame */
struct tcp_frm_rsp {
	unsigned short transID;
	unsigned short potoID;
	unsigned short msglen;
	unsigned char unitID;
	unsigned char fc;
	unsigned char byte;
}__attribute__((packed));
	
void build_rtu_frm(unsigned char *dst_buf, unsigned char *src_buf, unsigned char lenth);

int ser_query_parser(unsigned char *rx_buf, struct frm_para *sfpara);
int ser_resp_parser(unsigned char *rx_buf, struct frm_para *mfpara, int rlen);

int ser_build_query(unsigned char *tx_buf, struct frm_para *mfpara);
int ser_build_resp_excp(unsigned char *tx_buf, struct frm_para *sfpara, unsigned char excp_code);
int ser_build_resp_read_status(unsigned char *tx_buf, struct frm_para *sfpara, unsigned char fc);
int ser_build_resp_read_regs(unsigned char *tx_buf, struct frm_para *sfpara, unsigned char fc);
int ser_build_resp_set_single(unsigned char *tx_buf, struct frm_para *sfpara, unsigned char fc);

int tcp_query_parser(struct tcp_frm *rx_buf, struct tcp_frm_para *tsfpara);
int tcp_resp_parser(unsigned char *rx_buf, struct tcp_frm_para *tmfpara, int rlen);
int tcp_chk_pack_dest(struct tcp_frm *rx_buf, struct tcp_frm_para *tsfpara);

int tcp_build_query(struct tcp_frm *tx_buf, struct tcp_frm_para *tmfpara);
int tcp_build_resp_excp(struct tcp_frm_excp *tx_buf, struct tcp_frm_para *tsfpara, unsigned char excp_code);
int tcp_build_resp_read_status(struct tcp_frm_rsp *tx_buf, struct tcp_frm_para *tsfpara, unsigned char fc);
int tcp_build_resp_read_regs(struct tcp_frm_rsp *tx_buf, struct tcp_frm_para *tsfpara, unsigned char fc);
int tcp_build_resp_set_single(struct tcp_frm *tx_buf, struct tcp_frm_para *tsfpara, unsigned char fc);

#endif


