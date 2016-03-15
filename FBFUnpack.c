
#include <stdio.h>
#include <stdbool.h>


typedef int BOOL;
#define TRUE    (-1)
#define FALSE   (0)

typedef void VOID;
typedef unsigned char  UINT8;
typedef unsigned short UINT16;
typedef unsigned int   UINT32;


#define MAGIC_HEADER                                    "Marvell_FBF"
#define UNIQUE_SIZE                                     24
#define NUM_OF_SUPPORTED_FLASH_DEVS                     4
#define MAX_NUMBER_OF_FLASH_DEVICES_IN_MASTER_HEADER    4

typedef struct
{
    char acUnique[UNIQUE_SIZE];
    UINT16 ausFlash_Device_Spare_Area_Size[NUM_OF_SUPPORTED_FLASH_DEVS];

    UINT16 usFormat_Version;                                /* FBF format version */
    UINT16 usSize_of_Block;                                 /* un-used */
    UINT32 uiBytes_To_Program;                              /* Total number of bytes to program */
    UINT32 uiBytes_To_Verify;                               /* Total number of bytes to verify */
    UINT32 uiNumber_of_Bytes_To_Erase;                      /* Total number of bytes to erase */
    UINT32 uiMain_Commands;

    UINT32 uiNumOfDevices;                                  /* number of devices to burn in parallel */
    UINT32 uiDLerVersion;                                   /* Version of downloader current 1 */
    UINT32 auiDeviceHeaderOffset[MAX_NUMBER_OF_FLASH_DEVICES_IN_MASTER_HEADER]; /* offset in Sector 0 for each flash device header */
} MasterBlockHeader;


typedef struct
{
    UINT32 tCH;   /*Enable Signal Hold Time*/
    UINT32 tCS;   /*Enable Signal Setup Time*/
    UINT32 tWH;   /*ND_nWE high duration*/
    UINT32 tWP;   /*ND_nWE pulse width*/
    UINT32 tRH;   /*ND_nRE high duration*/
    UINT32 tRP;   /*ND_nRE pulse width*/
    UINT32 tR;    /*ND_nWE high to ND_nRE Low for Read*/
    UINT32 tWHR;  /*ND_nWE High to ND_nRE Low for Read Status*/
    UINT32 tAR;   /*ND_ALE Low to ND_nRE Low Delay*/
} FlashTimingStruct;

typedef struct
{
    UINT32 uiTotalNumOfBlocks;  /* The total number of blocks in flash */
    UINT32 uiFlashBlockSize;    /* Block size in Flash */
    UINT32 uiFlashOffset;       /* Flash offset address */
    UINT32 uiFlashBufferSize;   /* Buffer size in Flash */
    UINT32 uiNumberOfBlocksInBadBlockPool; /* What is the number of Blocks in the Bad Block Pool */
    UINT32 uiSpareAreaSize; /* Flash Device spare area size */
    UINT32 uiManufacturerBadBlockInfoOffset; /* Where manufacturer bad block information in flash */
} FlashSectionStruct;

#define MAX_NUMBER_OF_FLASH_SECTIONS    3

typedef struct
{
    FlashTimingStruct stFlashTiming;    /*Timing Parameters*/
    FlashSectionStruct stFlashSection[MAX_NUMBER_OF_FLASH_SECTIONS];
} FlashParametersStruct;

typedef UINT16      CORE_ID;
typedef UINT8       FLASH_FAMILY;

typedef struct
{
    UINT32 uiFlash_partition_size;    /* partition size of the image */
    UINT32 uiCommands;                /* bit switches */
    UINT32 uiFirst_Sector;            /* First sector of the image in the block device */
    UINT32 uiLength;                 /* Block length in bytes */
    UINT32 uiFlash_Start_Address;    /* start address in flash */
    UINT32 uiFlash_block_size;        /* flash device block size */
    UINT32 uiChecksumFormatVersion2;  /* new format version image checksum (left for backwards compatibility) */
} ImageStruct;

#define MAX_NUMBER_OF_IMAGE_STRUCTS_IN_DEVICE_HEADER   30

typedef struct
{
    UINT32 uiDeviceFlags;
    FlashParametersStruct stFlashParameters;    /* Device Parameters */

    UINT8 ucNumOfImages;        /* number of images */
    UINT8 ucBufferSizeLSB;      /* init to 0x00 */
    UINT16 usAccessWidth;       /* init to 0x0000 */
    CORE_ID usCore_ID;
    FLASH_FAMILY ucFlashFamily;   /*  Device Flags */
    UINT8 ucBufferSizeMSB;
    ImageStruct astImageStruct[MAX_NUMBER_OF_IMAGE_STRUCTS_IN_DEVICE_HEADER]; /* array of image structs */
} DeviceHeader;

#define MAX_RESEVERD_LEN    4
#define MAX_NUM_SKIP_BLOCKS     32

typedef struct
{
	UINT32 uiTotal_Number_Of_SkipBlocks; // total numbers of skip blocks
	UINT32 auiSkip_Blocks[MAX_NUM_SKIP_BLOCKS];
} SkipBlocksInfoStruct;

typedef struct
{
	UINT32 uiEraseAll; // erase all flag for user partition
    UINT32 uiResetBBT; // indicate if reset BBT
    UINT32 uiNandID;	 // nand flash ID
	UINT32 auiReserved[MAX_RESEVERD_LEN - 1];
	SkipBlocksInfoStruct stSkip_Blocks_Struct;
} FlashOptStruct;

typedef struct
{
    UINT32 uiImage_ID;				/* image id*/
    UINT32 uiImage_In_TIM;			/* indicate this image is in TIM or not*/
    UINT32 uiFlash_partition;        /* partition number of the image */
    UINT32 uiFlash_erase_size;      /* erase size of the image */
    UINT32 uiCommands;                /* bit switches */
    UINT32 uiFirst_Sector;            /* First sector of the image in the block device */
    UINT32 uiLength;                 /* Block length in bytes */
    UINT32 uiFlash_Start_Address;    /* start address in flash */
    UINT32 auiReserved[MAX_RESEVERD_LEN];
    UINT32 uiChecksumFormatVersion2;  /* new format version image checksum (left for backwards compatibility) */
} ImageStruct_11;

typedef struct
{
    UINT32 uiDeviceFlags;
    UINT32 uiDeviceParameters[16]; /* Device Parameters, reserve 16 U32 here, will be defined depending on different devices */
	FlashOptStruct stFlashOpt;
	UINT32 uiProductMode; // product mode
	UINT8 ucOptValue;	// choice: 0 - Not reset after download, 1 - Reset after download
	UINT8 ucChipID;
	UINT8 aucReserved[2];
	UINT32 auiReserved[MAX_RESEVERD_LEN - 2];
    UINT32 uiNumOfImages;        /* number of images */
    ImageStruct_11 astImageStruct_11[MAX_NUMBER_OF_IMAGE_STRUCTS_IN_DEVICE_HEADER]; /* array of image structs */
} DeviceHeader_11;

/* define DeviceHeader from FBF make version V2 (and down) to support backward compatibility for older versions */
typedef struct
{
    UINT8 ucNumOfImages;        /* number of images */
    UINT8 ucBufferSizeLSB;      /* init to 0x00 */
    UINT16 usAccessWidth;       /* init to 0x0000 */
    CORE_ID usCore_ID;
    FLASH_FAMILY ucFlashFamily;     /* Device Flags */
    UINT8 ucBufferSizeMSB;
    ImageStruct astImageStruct[MAX_NUMBER_OF_IMAGE_STRUCTS_IN_DEVICE_HEADER]; /* array of image structs */
} DeviceHeader_V2;

int main(int argc, const char **argv)
{
    const char szMagicHead[] = MAGIC_HEADER;
    const char *pszFileName = argv[1];
    FILE *hFBFStream = NULL;
    // char szBuffer[1024] = { 0 };
    size_t uzReadSum = 0;
    size_t uzIndex = 0;
    BOOL bValidFile = TRUE;
    UINT32 uiIndex = 0;
    UINT32 uiDeviceIndex = 0;
    UINT32 uiImageIndex = 0;
    ImageStruct *pstImageStruct = NULL;
    MasterBlockHeader stMasterBlockHeader = { 0 };
    DeviceHeader astDeviceHeader[MAX_NUMBER_OF_FLASH_DEVICES_IN_MASTER_HEADER] = { 0 };
    DeviceHeader_11 astDeviceHeader_11[MAX_NUMBER_OF_FLASH_DEVICES_IN_MASTER_HEADER] = { 0 };
    DeviceHeader_V2 astDeviceHeader_V2[MAX_NUMBER_OF_FLASH_DEVICES_IN_MASTER_HEADER] = { 0 };


    // fprintf(stdout, "argv%s")

    if (2 != argc) {
        fprintf(stderr, "\nUsage:");
        fprintf(stderr, "\n%s [fbf]\n", argv[0]);
        return 0;
    }

    // printf("sizeof(MasterBlockHeader)=%zu\n", sizeof(MasterBlockHeader));

    hFBFStream = fopen(pszFileName, "rb");
    if (NULL == hFBFStream) {
        fprintf(stderr, "Open file \"%s\" failed!\n", pszFileName);
        return -1;
    }

    uzReadSum = fread(&stMasterBlockHeader, 1, sizeof(MasterBlockHeader), hFBFStream);
    if (sizeof(MasterBlockHeader) != uzReadSum) {
        fprintf(stderr, "sizeof(MasterBlockHeader)=%zu,uzReadSum=%zu\n", sizeof(MasterBlockHeader), uzReadSum);
        goto InvalidFile;
    }

    for (uzIndex = 0; uzIndex < sizeof(szMagicHead); uzIndex++) {
        if (szMagicHead[uzIndex] != stMasterBlockHeader.acUnique[uzIndex]) {
            bValidFile = FALSE;
            break;
        }
    }
    
    if (FALSE == bValidFile) {
        goto InvalidFile;
    }

    // fprintf(stdout, "\n\n\n");
    fprintf(stdout, "MasterBlockHeader.usFormat_Version=%d\n", stMasterBlockHeader.usFormat_Version);
    fprintf(stdout, "MasterBlockHeader.usSize_of_Block=0x%08x\n", stMasterBlockHeader.usSize_of_Block);
    fprintf(stdout, "MasterBlockHeader.uiBytes_To_Program=0x%08x\n", stMasterBlockHeader.uiBytes_To_Program);
    fprintf(stdout, "MasterBlockHeader.uiBytes_To_Verify=0x%08x\n", stMasterBlockHeader.uiBytes_To_Verify);
    fprintf(stdout, "MasterBlockHeader.uiNumber_of_Bytes_To_Erase=0x%08x\n", stMasterBlockHeader.uiNumber_of_Bytes_To_Erase);
    fprintf(stdout, "MasterBlockHeader.uiMain_Commands=0x%08x\n", stMasterBlockHeader.uiMain_Commands);
    fprintf(stdout, "MasterBlockHeader.uiNumOfDevices=%d\n", stMasterBlockHeader.uiNumOfDevices);
    fprintf(stdout, "MasterBlockHeader.uiDLerVersion=%d\n", stMasterBlockHeader.uiDLerVersion);
    fprintf(stdout, "\n\n\n");

    if (0 != stMasterBlockHeader.uiNumOfDevices) {
        fprintf(stdout, "uiNumOfDevices=%d\n", stMasterBlockHeader.uiNumOfDevices);
    }

    for (uiDeviceIndex = 0; uiDeviceIndex < MAX_NUMBER_OF_FLASH_DEVICES_IN_MASTER_HEADER; uiDeviceIndex++) {
        fprintf(stdout, "MasterBlockHeader.auiDeviceHeaderOffset[%d]=0x%08x\n",
            uiDeviceIndex, stMasterBlockHeader.auiDeviceHeaderOffset[uiDeviceIndex]);
    }

    fprintf(stdout, "sizeof(MasterBlockHeader)=%zu\n", sizeof(MasterBlockHeader));

    fprintf(stdout, "sizeof(DeviceHeader)=%zu\n", sizeof(DeviceHeader));
    fprintf(stdout, "sizeof(DeviceHeader_11)=%zu\n", sizeof(DeviceHeader_11));
    fprintf(stdout, "sizeof(DeviceHeader_V2)=%zu\n", sizeof(DeviceHeader_V2));

    fprintf(stdout, "sizeof(ImageStruct)=%zu\n", sizeof(ImageStruct));
    fprintf(stdout, "sizeof(ImageStruct_11)=%zu\n", sizeof(ImageStruct_11));

    for (uiDeviceIndex = 0; uiDeviceIndex < stMasterBlockHeader.uiNumOfDevices; uiDeviceIndex++) {

#ifdef DEVHEADER
        fseek(hFBFStream, stMasterBlockHeader.auiDeviceHeaderOffset[uiDeviceIndex], 0);
        uzReadSum = fread(&astDeviceHeader[uiDeviceIndex], 1, sizeof(DeviceHeader), hFBFStream);
        if (sizeof(DeviceHeader) != uzReadSum) {
            fprintf(stderr, "sizeof(DeviceHeader)=%zu,uzReadSum=%zu\n", sizeof(DeviceHeader), uzReadSum);
            goto InvalidFile;
        }
        fprintf(stdout, "DeviceHeader[%d]\n", uiDeviceIndex);
        fprintf(stdout, "DeviceFlags=0x%08x, ", astDeviceHeader[uiDeviceIndex].uiDeviceFlags);
        fprintf(stdout, "NumOfImages=%d\n", astDeviceHeader[uiDeviceIndex].ucNumOfImages);

        fprintf(stdout, "BufferSizeLSB=0x%08x, BufferSizeMSB=0x%08x\n",
            astDeviceHeader[uiDeviceIndex].ucBufferSizeLSB, astDeviceHeader[uiDeviceIndex].ucBufferSizeMSB);
        fprintf(stdout, "usAccessWidth=0x%08x, usCore_ID=0x%08x, ucFlashFamily=0x%08x\n",
            astDeviceHeader[uiDeviceIndex].usAccessWidth, astDeviceHeader[uiDeviceIndex].usCore_ID,
            astDeviceHeader[uiDeviceIndex].ucFlashFamily);

        for (uiImageIndex = 0; uiImageIndex < astDeviceHeader[uiDeviceIndex].ucNumOfImages; uiImageIndex++) {
            pstImageStruct = &astDeviceHeader[uiDeviceIndex].astImageStruct[uiImageIndex];
            fprintf(stdout, "Dev[%d] Img[%02d] ", uiDeviceIndex, uiImageIndex);
            fprintf(stdout, "FirstSector=0x%08x, Length=0x%08x, FlashStartAddress=0x%08X\n",
                pstImageStruct->uiFirst_Sector, pstImageStruct->uiLength, pstImageStruct->uiFlash_Start_Address);
            fprintf(stdout, "               ");
            fprintf(stdout, "FlashPartitionSize=0x%08X,         Checksum=0x%08X\n",
                pstImageStruct->uiFlash_partition_size, pstImageStruct->uiChecksumFormatVersion2);
            //fprintf(stdout, "               ");
            //fprintf(stdout, "Commands=%02d, FlashBlockSize=0x%08x\n",
            //    pstImageStruct->uiCommands, pstImageStruct->uiFlash_block_size);
        }
#endif

#ifdef DEVHEADER_11
        fseek(hFBFStream, stMasterBlockHeader.auiDeviceHeaderOffset[uiDeviceIndex], 0);
        uzReadSum = fread(&astDeviceHeader_11[uiDeviceIndex], 1, sizeof(DeviceHeader_11), hFBFStream);
        if (sizeof(DeviceHeader_11) != uzReadSum) {
            fprintf(stderr, "sizeof(DeviceHeader_11)=%zu,uzReadSum=%zu\n", sizeof(DeviceHeader_11), uzReadSum);
            goto InvalidFile;
        }
        fprintf(stdout, "DeviceHeader[%d]\n", uiDeviceIndex);
        fprintf(stdout, "DeviceHeader[%d].uiDeviceFlags=0x%08x\n", uiDeviceIndex, astDeviceHeader_11[uiDeviceIndex].uiDeviceFlags);
        fprintf(stdout, "DeviceHeader[%d].uiNumOfImages=%d\n", uiDeviceIndex, astDeviceHeader_11[uiDeviceIndex].uiNumOfImages);
        
        fprintf(stdout, "uiImage_ID=0x%08x, Offset=0x%08x, Length=0x%08x, FlashStartAddress=0x%08x\n",
            astDeviceHeader_11[0].astImageStruct_11[0].uiImage_ID,
            astDeviceHeader_11[0].astImageStruct_11[0].uiFirst_Sector,
            astDeviceHeader_11[0].astImageStruct_11[0].uiLength,
            astDeviceHeader_11[0].astImageStruct_11[0].uiFlash_Start_Address);
#endif
#ifdef DEVHEADER_V2
        fseek(hFBFStream, stMasterBlockHeader.auiDeviceHeaderOffset[uiDeviceIndex], 0);
        uzReadSum = fread(&astDeviceHeader_V2[uiDeviceIndex], 1, sizeof(DeviceHeader_V2), hFBFStream);
        if (sizeof(DeviceHeader_V2) != uzReadSum) {
            fprintf(stderr, "sizeof(DeviceHeader_V2)=%zu, uzReadSum=%zu\n", sizeof(DeviceHeader_V2), uzReadSum);
            goto InvalidFile;
        }
        fprintf(stdout, "DeviceHeader[%d]\n", uiDeviceIndex);
        fprintf(stdout, "DeviceHeader[%d].ucNumOfImages=%d\n", uiDeviceIndex, astDeviceHeader_V2[uiDeviceIndex].ucNumOfImages);
        // fprintf(stdout, "DeviceHeader[%d].uiDeviceFlags")
#endif
    }

    fclose(hFBFStream);

    return 0;

InvalidFile:
    fprintf(stderr, "Error: Not a valid FBF file!\n");
    fclose(hFBFStream);
    return -1;
}
