#define ERR(fmt, ...) do {                                                  \
    dprintf(STDERR_FILENO, fmt "\n" __VA_OPT__(,) __VA_ARGS__);             \
    return -1;                                                              \
} while(0)

#define PACKET_EXTEND(PACK, LEN) ((PACK)->length + sizeof(size_t) * 2 + sizeof(tagged_parameter_t) + LEN)
#define MAC_TO_CHAR_P(_mac) (char*)(_mac),(char*)(_mac)+1,(char*)(_mac)+2,(char*)(_mac)+3,(char*)(_mac)+4,(char*)(_mac)+5

typedef uint8_t mac_t[6];

typedef struct __attribute__((__packed__)) RadioHeader {
  uint8_t revision;
  uint8_t pad;
  uint16_t length;
  unsigned long long TSFT:1, Flags:1, Rate:1, Channel:1, FHSS:1, Signal:1, other_flag:57, Ext:1;
  uint8_t fields[];
} radio_header_t;

typedef struct __attribute__((__packed__)) Body {
  unsigned version:2, type:2, subtype:4, flags:8;
  uint16_t duration;
  mac_t receiver;
  mac_t trasmitter;
  mac_t bss_id;
} body_t;

