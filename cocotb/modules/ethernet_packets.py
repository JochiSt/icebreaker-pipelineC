# create UDP data to be transmitted via cocotb ethernet
import struct

class Ethernet_packet(object):
    IPv4_DATAGRAM = 0x0800
    ARP = 0x0806

    def __init__(self, SRC_MAC, DST_MAC):
        self.bytes = bytearray()
        # Ethernet Header
        self.bytes += struct.pack("!HIHIH",
                                  DST_MAC >> 32,
                                  DST_MAC & 0xFFFFFFFF,
                                  SRC_MAC >> 32,
                                  SRC_MAC & 0xFFFFFFFF,
                                  self.IPv4_DATAGRAM)

    def print(self):
        for i, byte in enumerate(self.bytes):
            if i % 4 == 0 and i > 0:
                print("")
            print("%02X"%(byte), end=" ")
        print()

    def payload(self):
        return self.bytes

    # the following function is heavily inspired by
    # https://www.codeproject.com/Tips/612847/Generate-a-quick-and-easy-custom-pcap-file-using-P
    def write_pcap(self, file_name):
        #Global header for pcap 2.4
        pcap_global_header =   ('D4 C3 B2 A1'
                                '02 00'         #File format major revision (i.e. pcap <2>.4)
                                '04 00'         #File format minor revision (i.e. pcap 2.<4>)
                                '00 00 00 00'
                                '00 00 00 00'
                                'FF FF 00 00'
                                '01 00 00 00')

        #pcap packet header that must preface every packet
        pcap_packet_header =   ('AA 77 9F 47'
                                '90 A2 04 00'
                                'XX XX XX XX'   #Frame Size (little endian)
                                'YY YY YY YY')  #Frame Size (little endian)

        import binascii

        pcap_len = len(self.bytes)

        hex_str = "%08x"%int(pcap_len)

        reverse_hex_str = hex_str[6:] + hex_str[4:6] + hex_str[2:4] + hex_str[:2]
        pcaph = pcap_packet_header.replace('XX XX XX XX',reverse_hex_str)
        pcaph = pcaph.replace('YY YY YY YY',reverse_hex_str)

        bytestring = pcap_global_header + pcaph + " ".join([ "%02X"%(b) for b in self.bytes ])

        bytelist = bytestring.split()
        print(bytelist)
        bytes = binascii.a2b_hex(''.join(bytelist))
        bitout = open(file_name, 'wb')
        bitout.write(bytes)


class IP_packet(Ethernet_packet):
    def __init__(self, SRC_MAC, DST_MAC, SRC_IP, DST_IP, data_length=0, ttl=10):
        super().__init__(SRC_MAC, DST_MAC)

        version_ihl = 0x45
        TOS = 0x00

        total_length = data_length + 5

        identification = 0x00
        flags_fragmentoffset = 0b010_000000000000

        header_checksum = 0x00  # ignore header checksum

        TTL = ttl     # hop count
        protocol = 17 # UDP

        # IP header
        self.bytes += struct.pack("!BBH", version_ihl, TOS, total_length)
        self.bytes += struct.pack("!HH", identification, flags_fragmentoffset)
        self.bytes += struct.pack("!BBH", TTL, protocol, header_checksum)
        self.bytes += struct.pack("!II", SRC_IP, DST_IP)

class UDP_packet(IP_packet):


    def __init__(self, SRC_MAC, DST_MAC, SRC_IP, DST_IP, SRC_PORT, DST_PORT, data):
        super().__init__(SRC_MAC, DST_MAC, SRC_IP, DST_IP, len(data))
        # UDP header
        self.bytes += struct.pack("!HHHH", SRC_PORT, DST_PORT, len(data), 0x00)



if __name__ == "__main__":


    udp_packet = UDP_packet(
        0x12_34_56_78_90_AB,
        0x12_34_56_78_90_AC,
        192_168_1_4,
        192_168_1_5,
        0x1234,
        0x1234,
        "test1234"
        )

    udp_packet.print()

    udp_packet.write_pcap('output.pcap')
