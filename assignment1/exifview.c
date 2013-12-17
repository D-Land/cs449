#include <stdio.h>
#include <string.h>

struct header_data{
  unsigned short sof_marker;
  unsigned short app1_marker;
  unsigned short app1_size;
  char exif_string[4];
  unsigned short null_terminator;
  char endianness[2];
  unsigned short version_number;
  unsigned int offset_to_exif_block;
  unsigned short tag_count;
};

struct tag_data{
  unsigned short tag_id;
  unsigned short data_type;
  unsigned int num_of_items;
  unsigned int offset;
};

struct pic_data{
  char make[50];
  char model[50];
  int width;
  int height;
  int iso;
  unsigned int expo_speed[2];
  unsigned int f_stop[2];
  unsigned int focal_leng[2];
  char date[20];
};

int main(int argc, char *argv[]) {
  FILE *file;
  struct header_data header;
  struct tag_data tag;
  struct pic_data pic;
  int i = 0;
  unsigned int ftell_loc;
  unsigned int ftell_loc_2;
  unsigned short sub_block_count;
  double f_stopp;

  if(argc != 2){
    printf("Usage: %s filename", argv[0]);
    return 1;
  }

  file = fopen(argv[1], "rb");
  if(!file){
    printf("\n%s cannot be opened.\n", argv[1]);
    return 1;
  }

  fread(&header, sizeof(header), 1, file);

  if(header.sof_marker != 0xd8ff){
    printf("\nError: File is not of the correct type.\n");
    return 1;
  }

  if(header.app1_marker != 0xe1ff){
    printf("\nError: File is not of the correct type.\n");
    return 1;
  }

  if(0 != strcmp(header.exif_string, "Exif")){
    printf("\nError: File is not of the correct type.\n");
    return 1;
  }

  if(0 != strcmp(header.endianness, "II*")){
    printf("\nError: File is not little endian.\n");
    return 1;
  }

  fseek(file, 22, SEEK_SET);
  for(i = 0; i < header.tag_count; i++){
    fread(&tag, sizeof(tag), 1, file);

    if(tag.tag_id == 0x010f){
      ftell_loc = ftell(file);
      fseek(file, tag.offset + 12, SEEK_SET);
      fread(&pic.make, tag.num_of_items, 1, file);
      fseek(file, ftell_loc, SEEK_SET);
    }

    if(tag.tag_id == 0x0110){
      ftell_loc = ftell(file);
      fseek(file, tag.offset + 12, SEEK_SET);
      fread(&pic.model, tag.num_of_items, 1, file);
      fseek(file, ftell_loc, SEEK_SET);
    }

    if(tag.tag_id == 0x8769){
      fseek(file, tag.offset + 12, SEEK_SET);
      ftell_loc = ftell(file);

      fread(&sub_block_count, sizeof(sub_block_count), 1, file);
      for(i=0; i < sub_block_count; i++){
        fread(&tag, sizeof(tag), 1, file);
        if(tag.tag_id == 0xA002){
          pic.width = tag.offset;
          printf("Width: %d\n", tag.offset);
        }

        if(tag.tag_id == 0xA003){
          pic.height = tag.offset;
        }

        if(tag.tag_id == 0x8827){
          pic.iso = tag.offset;
        }

        if(tag.tag_id == 0x829a){
          ftell_loc_2 = ftell(file);
          fseek(file, tag.offset + 12, SEEK_SET);
          fread(&pic.expo_speed, sizeof(pic.expo_speed), 1, file);
          fseek(file, ftell_loc_2, SEEK_SET);
        }

        if(tag.tag_id == 0x829d){
          ftell_loc_2 = ftell(file);
          fseek(file, tag.offset + 12, SEEK_SET);
          fread(&pic.f_stop, sizeof(pic.f_stop), 1, file);
          fseek(file, ftell_loc_2, SEEK_SET);
        }

        if(tag.tag_id == 0x920A){
          ftell_loc_2 = ftell(file);
          fseek(file, tag.offset + 12, SEEK_SET);
          fread(&pic.focal_leng, sizeof(pic.focal_leng), 1, file);
          fseek(file, ftell_loc_2, SEEK_SET);
        }

        if(tag.tag_id == 0x9003){
          ftell_loc_2 = ftell(file);
          fseek(file, tag.offset + 12, SEEK_SET);
          fread(pic.date, sizeof(pic.date), 1, file);
          fseek(file, ftell_loc_2, SEEK_SET);
        }
      }
      break;
    }
  }
  printf("Manufacturer:   %s\n", pic.make);
  printf("Model:          %s\n", pic.model);
  printf("Exposure Time:  %d/%d Seconds\n", pic.expo_speed[0], pic.expo_speed[1]);
  printf("F-stop:         f/%f\n",(double) pic.f_stop[0]/pic.f_stop[1]);
  printf("ISO:            ISO%d\n", pic.iso);
  printf("Date Taken:     %s\n", pic.date);
  printf("Focal Length:   %dmm\n", pic.focal_leng[0]/pic.focal_leng[1]);
  printf("Width:          %d px\n", pic.width);
  printf("Height:         %d px\n", pic.height);

  return 0;
}

