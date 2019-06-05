#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include <stdint.h>

/* clang-format off */
#define PROTOCOL_VERSION                (1)
#define PROTOCOL_BUF_LEN                (1024*2)//Ŀǰ������ݰ�������1500�ֽ�

#define PROTOCOL_RET_SUCC               (0)
#define PROTOCOL_RET_FAIL               (1)

#define PROTOCOL_TYPE_STR               (1)

#define PROTOCOL_DEBUG                  (0)

#define PROTOCOL_PKT_CRC                (0)

#define PROTOCOL_MSG_MAX_LEN            (64)    //msg�ֶ���󲻵�64

typedef enum _protocol_type
{
    PROTOCOL_TYPE_INIT_DONE             = (0),

    PROTOCOL_TYPE_PKT_PRASE_RET         = (1),

    PROTOCOL_TYPE_SET_CONFIG            = (2),//��������ݣ�1����������ֵ��bese64)��2���������꼰ID��3�������ʣ�4��IO�����ʱ
    PROTOCOL_TYPE_SET_CONFIG_RET        = (3),

    PROTOCOL_TYPE_GET_CONFIG            = (4),
    PROTOCOL_TYPE_GET_CONFIG_RET        = (5),

    PROTOCOL_TYPE_CAL_PIC_FEA           = (6),
    PROTOCOL_TYPE_CAL_PIC_FEA_RET       = (7),

    PROTOCOL_TYPE_DEL_BY_UID            = (8),
    PROTOCOL_TYPE_DEL_BY_UID_RET        = (9),

    PROTOCOL_TYPE_FACE_INFO             = (10),

    PROTOCOL_TYPE_QUERY_FACE_INFO       = (11),
    PROTOCOL_TYPE_QUERY_FACE_INFO_RET   = (12),

    PROTOCOL_MAX
} protocol_type;

typedef struct _pkt_head
{
    uint8_t pkt_json_start_1;
    uint8_t pkt_json_start_2;

    uint8_t pkt_json_end_1;
    uint8_t pkt_json_end_2;

    uint8_t pkt_jpeg_start_1;
    uint8_t pkt_jpeg_start_2;

    uint8_t pkt_jpeg_end_1;
    uint8_t pkt_jpeg_end_2;
} pkt_head_t;

typedef struct _brd_cfg
{
    int uart_baud;
    int open_delay;
    int pkt_fix;

    int out_feature;
    int out_interval_in_ms;

    int auto_out_feature;
} brd_cfg;

typedef struct _pic_fea
{
    int uid_len;
    uint8_t *uid;

    int fea_len;
    uint8_t *feature;
} pic_fea_t;

typedef struct _qurey_face_info
{
    int order;
    uint8_t uid[16+1];
    uint8_t *fea;
	int fea_len;
} qurey_face_info_t;

typedef void (*qurey_face_info_del)(void *ctx);

typedef struct _qurey_face_infos
{
    int face_num;
    qurey_face_info_del del;//�����ͷ�������ڴ�

    qurey_face_info_t **face_info;
} qurey_face_infos_t;

typedef struct _qurey_face_ret
{
    int total;
    int start;
    int end;

    qurey_face_infos_t face_infos;

} qurey_face_ret_t;

typedef struct _face_info
{
    char* pic_no;

    int total;
    int current;
    int x1;
    int y1;
    int x2;
    int y2;

    float score;

    int uid_len;
    uint8_t *uid;

    int fea_len;
    uint8_t *fea;

} face_info_t;

typedef struct _pkt_callback
{
    int cb_type;
    int code;
    char *msg;
    void *ctx;
} pkt_callback_t;

typedef int (*protocol_prase_pkt_callback)(void *ctx);
/* clang-format on */

extern char *protocol_str[PROTOCOL_MAX + 1];

extern pkt_head_t g_pkt_head; //�������ݰ�У��ʱʹ��

extern uint8_t uart_recv_char;
extern uint8_t recv_over_flag;

uint16_t crc_check(uint8_t *data, uint32_t length);
void hex_str(uint8_t *inchar, uint16_t len, uint8_t *outtxt);
uint16_t str_hex(uint8_t *str, uint8_t *hex);

void protocol_init(void);
void protocol_register_cb(protocol_prase_pkt_callback cb);
void protocol_unregister_cb(void);

void protocol_prase(void);

uint8_t protocol_send_set_cfg(brd_cfg *board_cfg);

uint8_t protocol_send_get_cfg(void);

uint8_t protocol_send_cal_pic_fea(int pic_size, int auto_add, uint8_t *pic_sha256);

uint8_t protocol_send_del_user_by_uid(uint8_t del_all, uint8_t uid[16 + 1]);

uint8_t protocol_send_query_face(int get_total, int start, int end, int out_feature);

#endif
