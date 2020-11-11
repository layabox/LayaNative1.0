/**
@file			JCPngImg.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/

#include <string.h>
#include "png.h"
#include "pngconf.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "../util/JCCommonMethod.h"
#include "JCImageRW.h"

//------------------------------------------------------------------------------
#pragma warning (disable: 4996)
namespace laya
{
    //static const png_byte mng_LAYA[5] = { 76,  65,  89, 65, (png_byte) '\0' };
    static const png_byte mng_LAYA[5] = { 105,  84,  88, 116, (png_byte) '\0' };
#ifdef PNG_UNKNOWN_CHUNKS_SUPPORTED
    png_byte unused_chunks[] =
    {
        104,  73,  83,  84, (png_byte) '\0',   /* hIST */
        //105,  84,  88, 116, (png_byte) '\0',   /* iTXt */
        112,  67,  65,  76, (png_byte) '\0',   /* pCAL */
        115,  67,  65,  76, (png_byte) '\0',   /* sCAL */
        115,  80,  76,  84, (png_byte) '\0',   /* sPLT */
#if !defined(PNG_tIME_SUPPORTED)
        116,  73,  77,  69, (png_byte) '\0',   /* tIME */
#endif
#ifdef PNG_APNG_SUPPORTED /* libpng was built with APNG patch; */
                                               /* ignore the APNG chunks */
                                               97,  99,  84,  76, (png_byte) '\0',   /* acTL */
                                               102,  99,  84,  76, (png_byte) '\0',   /* fcTL */
                                               102, 100,  65,  84, (png_byte) '\0',   /* fdAT */
#endif
    };
#endif
    static int read_user_chunk_callback(png_struct *ping, png_unknown_chunkp chunk)
    {
        /* The unknown chunk structure contains the chunk data:
        png_byte name[5];
        png_byte *data;
        png_size_t size;

        Note that libpng has already taken care of the CRC handling.

        Returns one of the following:
        return(-n);  chunk had an error
        return(0);  did not recognize
        return(n);  success
        */
        /* LAYA */
        if (chunk->name[0] == 'i' &&
            chunk->name[1] == 'T' &&
            chunk->name[2] == 'X' &&
            chunk->name[3] == 't')
        {
            if (memcmp("LAYA", chunk->data, 4) == 0)
            {
                BitmapData* pBitmapData = (BitmapData*)png_get_user_chunk_ptr(ping);
                if (chunk->size > 9)
                {
                    pBitmapData->m_pUserData = new char[chunk->size - 9];
                    memcpy(pBitmapData->m_pUserData, (const char*)chunk->data + 9, chunk->size - 9);
                    return(1);
                }
            }
        }
        return(0); /* Did not recognize */
    }
	typedef struct  
	{
		unsigned char* data;
		int size;
		int offset;
	}ImageSource;

	//从内存读取PNG图片的回调函数
	static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		ImageSource* isource = (ImageSource*)png_get_io_ptr(png_ptr);
		if( (int)(isource->offset + length) <= isource->size)
		{
			memcpy(data, isource->data+isource->offset, length);
			isource->offset += length;
		}
		else
			png_error(png_ptr, "pngReaderCallback failed");
	}
	void my_error_fn(png_structp png_ptr, png_const_charp error_msg)
	{
		
	}
	void my_warning_fn(png_structp png_ptr,png_const_charp warning_msg)
	{
		
	}

    static unsigned int multiply_alpha(unsigned int  alpha, unsigned int  color)
    {
        unsigned int  temp = alpha * color + 0x80;
        return (temp + (temp >> 8)) >> 8;
    }

    static void premultiply_data(png_structp png, png_row_infop row_info, png_bytep data)
    {
        unsigned int  i;

        for (i = 0; i < row_info->rowbytes; i += 4)
        {
            unsigned char*  base = &data[i];
            unsigned int    alpha = base[3];


            if (alpha == 0)
                base[0] = base[1] = base[2] = base[3] = 0;

            else
            {
                unsigned int  red = base[0];
                unsigned int  green = base[1];
                unsigned int  blue = base[2];


                if (alpha != 0xFF)
                {
                    red = multiply_alpha(alpha, red);
                    green = multiply_alpha(alpha, green);
                    blue = multiply_alpha(alpha, blue);
                }

                base[0] = (unsigned char)red;
                base[1] = (unsigned char)green;
                base[2] = (unsigned char)blue;
                base[3] = (unsigned char)alpha;
            }
        }
    }
	unsigned char* ReadPNGFromMem(BitmapData* pBitmapData, unsigned char* data, int dataSize)
	{
		//unsigned char sig[8];
		png_structp png_ptr;  
		png_infop   info_ptr;  
		unsigned char* image_data;  
		int bit_depth;  
		int color_type;  
		unsigned int rowbytes;  
		png_uint_32 i;  
		png_bytepp row_pointers;  

		if ((png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, my_error_fn, my_warning_fn)) == NULL) {  
			return NULL;  
		}  

#if defined(PNG_UNKNOWN_CHUNKS_SUPPORTED)
        /* Ignore unused chunks and all unknown chunks except for caNv and vpAg */
# if PNG_LIBPNG_VER < 10700 /* Avoid libpng16 warning */
        png_set_keep_unknown_chunks(png_ptr, 2, NULL, 0);
# else
        png_set_keep_unknown_chunks(png_ptr, 1, NULL, 0);
# endif
        png_set_keep_unknown_chunks(png_ptr, 2, (png_bytep)mng_LAYA, 1);
        png_set_keep_unknown_chunks(png_ptr, 1, unused_chunks, (int)sizeof(unused_chunks) / 4);
        /* Callback for other unknown chunks */
        png_set_read_user_chunk_fn(png_ptr, pBitmapData, read_user_chunk_callback);
#endif

		if ((info_ptr = png_create_info_struct(png_ptr)) == NULL) {  
			png_destroy_read_struct(&png_ptr, NULL, NULL);  
			return NULL;  
		}  

		if (setjmp(png_jmpbuf(png_ptr))) {  
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);  
			return NULL;  
		}  

		ImageSource imgsource;
		imgsource.data = data;
		imgsource.size = dataSize;
		imgsource.offset = 0;
		png_set_read_fn(png_ptr, &imgsource,pngReadCallback);
		png_read_info(png_ptr, info_ptr); 	
		png_set_sig_bytes(png_ptr, 8);  
        png_uint_32 width = 0;
        png_uint_32 height = 0;
		png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

        pBitmapData->m_nWidth = width;
        pBitmapData->m_nHeight = height;

        pBitmapData->m_nOffsetX = 0.0f;
        pBitmapData->m_nOffsetY = 0.0f;
        pBitmapData->m_nTextureWidth = width;
        pBitmapData->m_nTextureHeight = height;

        //预乘alpha
        //png_set_read_user_transform_fn(png_ptr, premultiply_data);

		//if (color_type & PNG_COLOR_MASK_ALPHA)
		//    png_set_strip_alpha(png_ptr);  //去掉alhpa信息了。TODO 以后要加上。
		if (bit_depth >8)
			png_set_strip_16(png_ptr);  //16bit的通道要转换成8bit的
		if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)  
			png_set_gray_to_rgb(png_ptr);  
		if (color_type == PNG_COLOR_TYPE_PALETTE)  
			png_set_palette_to_rgb(png_ptr);  
		if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
			png_set_tRNS_to_alpha(png_ptr);
		// if (color_type == PNG_COLOR_TYPE_RGB)
		png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);//PNG_FILLER_BEFORE);

		png_read_update_info(png_ptr, info_ptr);  

		rowbytes = png_get_rowbytes(png_ptr, info_ptr);  
		if ((image_data =(unsigned char *) malloc(height * rowbytes)) == NULL) {
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);  
			return NULL;  
		}  
		if ((row_pointers =(png_bytepp) malloc(height * sizeof(png_bytep))) == NULL) {
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);  
			free(image_data);  
			return NULL;  
		}  

		for (i = 0; i < height; i++)
			//row_pointers[*h - 1 - i] = image_data + i*rowbytes;
			row_pointers[i] = image_data + i*rowbytes;

		png_read_image(png_ptr, row_pointers);  
		free(row_pointers);  
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);  

		return image_data;  
	}
	//获得宽高信息，并不最终解码
	/*bool LoadPngWH( const char* p_sFileName,int& p_nWidth,int& p_nHeight )
	{
		FILE* f;
		unsigned char sig[8];
		png_structp png_ptr;
		png_infop   info_ptr;
		//unsigned char* image_data;
		int bit_depth;
		int color_type;
		//unsigned int rowbytes;
		//png_uint_32 i;
		//png_bytepp row_pointers;

		if ((f = fopen(p_sFileName, "rb")) == NULL)
			return false;
		fread(sig, sizeof(*sig), sizeof(sig), f);
		if (!png_check_sig(sig, sizeof(*sig))) {
			fclose(f);
			return false;
		}
		if ((png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)) == NULL) {
			fclose(f);
			return false;
		}
		if ((info_ptr = png_create_info_struct(png_ptr)) == NULL) {
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			fclose(f);
			return false;
		}

		if (setjmp(png_jmpbuf(png_ptr))) {
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			fclose(f);
			return false;
		}
		png_ptr->io_ptr = (png_voidp) f;
		png_set_sig_bytes(png_ptr, 8);

		png_read_info(png_ptr, info_ptr);
		png_uint_32 w,h;
		png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type, NULL, NULL, NULL);
		p_nWidth = w;
		p_nHeight = h;
		fclose(f);
		return true;
	}*/
	bool SavePng(char* png_file_name, char **data, int width, int height, int bit_depth)  
	{  
		png_structp png_ptr;  
		png_infop info_ptr;  
		FILE *png_file = fopen(png_file_name, "wb");  
		//assert(png_file);  
		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);  
		if(png_ptr == NULL)  
		{  
			//cerr<<"ERROR:png_create_write_struct/n";
            if (png_file)
                fclose(png_file);
			return false;  
		}  
		info_ptr = png_create_info_struct(png_ptr);  
		if(info_ptr == NULL)  
		{  
			//cerr<<"ERROR:png_create_info_struct/n";
            if (png_file) {
                fclose(png_file);
            }
			png_destroy_write_struct(&png_ptr, NULL);  
			return false;  
		}  
		png_init_io(png_ptr, png_file);  
		png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, PNG_COLOR_TYPE_RGB_ALPHA,  
			PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);  
		//png_set_PLTE(png_ptr, info_ptr, palette, palette_len);  
		png_write_info(png_ptr, info_ptr);  

		png_bytepp row_pointers = new png_bytep[height];  
		for (int i=0; i<height; ++i)  
		{  
			row_pointers[i] = (png_bytep)data[i];  
		}  
		png_write_image(png_ptr, row_pointers);  
		delete[] row_pointers;
		png_write_end(png_ptr, info_ptr);  
		//png_free(png_ptr, palette);  
		//palette=NULL;  
		png_destroy_write_struct(&png_ptr, &info_ptr);  
		fclose(png_file);  
		return true;  
	}  
    typedef unsigned char ui8;
    static void PngWriteToMemoryFunc(png_structp  png_ptr, png_bytep data, png_size_t length) {
        std::vector<ui8> *p = (std::vector<ui8>*)png_get_io_ptr(png_ptr);
        p->insert(p->end(), data, data + length);
    }

    std::pair<unsigned char*, unsigned long> convertBitmapToPng(const char* p_pData, int nWidth, int nHeight, int nBitDepth){
        std::vector<unsigned char> buffer;
        buffer.reserve(nWidth * nHeight * 4);


        std::pair<unsigned char*, unsigned long> res
            = std::make_pair((unsigned char*)NULL, 0);

        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, (png_voidp)NULL, NULL, NULL);
        if (!png_ptr)
            return res;
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
            return res;

        if (setjmp(png_jmpbuf(png_ptr)))
            return res;

        png_set_write_fn(png_ptr, &buffer, PngWriteToMemoryFunc, NULL);


        png_set_IHDR(png_ptr, info_ptr,
            nWidth,
            nHeight,
            8,
            PNG_COLOR_TYPE_RGB_ALPHA,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT);

        png_write_info(png_ptr, info_ptr);
        for (int i = 0; i < nHeight; i++)
            png_write_row(png_ptr, (unsigned char*)p_pData + i * nWidth * 4);

        png_write_end(png_ptr, info_ptr);

        png_destroy_write_struct(&png_ptr, &info_ptr);

        unsigned long nByte = buffer.size();
        unsigned char* pBuffer = new unsigned char[nByte];
        memcpy(pBuffer, &buffer[0], nByte);
        res.first = pBuffer;
        res.second = nByte;
        return res;
    }
   


}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
