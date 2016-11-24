#include <xinu.h>

static struct fsystem fsd;
int dev0_numblocks;
int dev0_blocksize;
char *dev0_blocks;

extern int dev0;

char block_cache[512];

#define SB_BLK 0
#define BM_BLK 1
#define RT_BLK 2

#define NUM_FD 16
struct filetable oft[NUM_FD];
int next_open_fd = 0;


#define INODES_PER_BLOCK (fsd.blocksz / sizeof(struct inode))
#define NUM_INODE_BLOCKS (( (fsd.ninodes % INODES_PER_BLOCK) == 0) ? fsd.ninodes / INODES_PER_BLOCK : (fsd.ninodes / INODES_PER_BLOCK) + 1)
#define FIRST_INODE_BLOCK 2

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock);

/* specify the block number to be set in the mask */
int fs_setmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  fsd.freemask[mbyte] |= (0x80 >> mbit);
  return OK;
}

/* specify the block number to be read in the mask */
int fs_getmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  return( ( (fsd.freemask[mbyte] << mbit) & 0x80 ) >> 7);
  return OK;

}

/* specify the block number to be unset in the mask */
int fs_clearmaskbit(int b) {
  int mbyte, mbit, invb;
  mbyte = b / 8;
  mbit = b % 8;

  invb = ~(0x80 >> mbit);
  invb &= 0xFF;

  fsd.freemask[mbyte] &= invb;
  return OK;
}


/* YOUR CODE GOES HERE */
int fs_create(char *filename, int mode) {
  struct directory *dir = &fsd.root_dir;
  int dev=0;
  struct inode *inode_data=(struct inode *)getmem(sizeof(struct inode));

  if(fsd.inodes_used >= INODEBLOCKS) {
    printf("ERR: Inodes used\n");
    return SYSERR;
  }

  fsd.inodes_used++;

  if (dir->numentries >= DIRECTORY_SIZE) {
    printf("ERR: Num entry used\n");
    return SYSERR;
  }

  struct dirent *dir_ent;
  dir_ent = &dir->entry[dir->numentries];
  dir_ent->inode_num = fsd.inodes_used-1;
  dir->numentries++;
  // printf("NUM INODE: %d\n", dir_ent->inode_num);
  strncpy(dir_ent->name, filename, strlen(filename));

  inode_data->id = dir_ent->inode_num;
  inode_data->device = dev;
  inode_data->size = 0;

  if(fs_put_inode_by_num(dev, dir_ent->inode_num, inode_data) == SYSERR) {
    printf("ERR: Create Inode failed\n");
    return SYSERR;
  }

  return OK;

}
int get_new_free_block() {
  int i=NUM_INODE_BLOCKS+2;
  while(i<fsd.nblocks) {
    if(fs_getmaskbit(i) == 0) {
      return i;
    }
    i++;
  }
  return -1;
}

int fs_close(int fd) {
  if(fd < 0 || fd >= NUM_FD) {
    printf("FD out of bounds\n");
    return SYSERR;
  }
  struct filetable *ft=&oft[fd];
  ft->state=FSTATE_CLOSED;
  return OK;
}

int fs_open(char *filename, int flags) {
  struct directory *dir = &fsd.root_dir;
  int dev=0;
  struct inode *inode_data=(struct inode *)getmem(sizeof(struct inode));
  struct dirent *dir_ent=NULL;
  int file_inode_num = 0;
  struct filetable *cur_ft;
  int i=0;
  // printf("In OPEN\n");
  // printf("Searching for %s\n", filename);
  // printf("Search ret: %d", strncmp(dir->entry[0].name, filename, strlen(filename)));

  printf("dir->numentries: %d\n", dir->numentries);
  for(i=0;i<dir->numentries;i++) {
    dir_ent = &dir->entry[i];
    // printf("i: %d\n", i);
    // printf("File name: %s\n", dir_ent->name);
    if(strncmp(dir_ent->name, filename, strlen(filename)) == 0) {
      file_inode_num = dir_ent->inode_num;
      printf("Name matched\n"); 
    }
  }

  if(dir_ent == NULL) {
    printf("ERR: File not found\n");
    return SYSERR;
  }

  if(file_inode_num == -1) {
    printf("ERR: Inode Num failed\n");
    return SYSERR;
  }

  printf("OPEN: Fething Inode by Num: %d\n", file_inode_num);
  if(fs_get_inode_by_num(dev, file_inode_num, inode_data) == SYSERR) {
    printf("ERR: Inode Get failed\n");
    return SYSERR;
  }

  if(next_open_fd >= NUM_FD) {
    printf("ERR: Open FD failed\n");
    return SYSERR;
  }
  printf("OPEN: File size: %d\n", inode_data->size);
  printf("OPEN: Inode Id: %d\n", inode_data->id);
  cur_ft = &oft[next_open_fd++];
  cur_ft->state = FSTATE_OPEN;
  cur_ft->de = dir_ent;
  cur_ft->in = *inode_data;
  return next_open_fd-1;
}

int fs_seek(int fd, int offset) {
  struct filetable *ftptr;

  if(fd < 0 || fd >= NUM_FD) {
    printf("FD out of bounds\n");
    return SYSERR;
  }

  printf("FD: %d\n", fd);
  ftptr=&oft[fd];
  if(ftptr->fileptr + offset < 0 || ftptr->fileptr + offset > ftptr->in.size) {
    printf("Invalid offset\n");
    return SYSERR;
  }
  ftptr->fileptr = ftptr->fileptr + offset;
  return OK;
}

int fs_read(int fd, void *buf, int nbytes) {
  int len=nbytes;
  int cur_block=0,cur_offset=0,cur_len=0;
  int diskblock=0;
  struct filetable *ftptr;
  int total_read_len = 0;
  if(fd < 0 || fd >= NUM_FD) {
    printf("FD out of bounds\n");
    return SYSERR;
  }

  // printf("FD: %d\n", fd);
  ftptr=&oft[fd];

  if(ftptr->state != FSTATE_OPEN) {
    printf("FD not open bounds\n");
    return SYSERR;
  }

  if(len > ftptr->in.size - ftptr->fileptr) {
    len = ftptr->in.size - ftptr->fileptr;
  }

  while(len > 0) {
    cur_block = ftptr->fileptr / fsd.blocksz;
    cur_offset = ftptr->fileptr % fsd.blocksz;

    if(len < fsd.blocksz - cur_offset) {
      cur_len = len;
    } else {
      cur_len = fsd.blocksz - cur_offset;
    }

    // printf("cur_block: %d\n", cur_block);
    diskblock = fs_fileblock_to_diskblock (0, fd, cur_block);
    // printf("diskblock: %d\n", diskblock);
    bs_bread(dev0, diskblock, cur_offset, &block_cache[0], cur_len);

    //Copy the actual content
    memcpy((buf+total_read_len),block_cache, cur_len);
    total_read_len = total_read_len+cur_len;

    len = len - cur_len;

    //Update Filetable
    ftptr->fileptr=ftptr->fileptr+cur_len;
  }

  return total_read_len;
}

int fs_write(int fd, void *buf, int nbytes) {
  int len=nbytes,cur_block=0,cur_len=0,cur_offset=0;
  struct filetable *ftptr;
  int total_write_len = 0;

  if(fd < 0 || fd >= NUM_FD) {
    printf("FD out of bounds\n");
    return SYSERR;
  }

  ftptr=&oft[fd];

  if(ftptr->state != FSTATE_OPEN) {
    printf("FD not open bounds\n");
    return SYSERR;
  }

  // printf("File PTR");
  // printf("%d\n", ftptr->fileptr);
  // printf("File INODE used ");
  // printf("%d\n", fsd.inodes_used);
  // printf("File INODE");
  // printf("%d\n", ftptr->de->inode_num);
  // printf("Data to write");
  // printf("%s\n", buf);

  // printf("fsd.blocksz: %d\n", fsd.blocksz);
  
  while(len > 0) {

    // Find how much is left in the current block and get a new block if needed
    cur_block = ftptr->fileptr/fsd.blocksz;
    cur_offset = ftptr->fileptr % fsd.blocksz;

    // printf("cur_block: %d\n", cur_block);
    // printf("cur_offset: %d\n", cur_offset);
    
    
    if(cur_offset == 0) {
      cur_block = get_new_free_block();
      if(cur_block == -1) {
        printf("Couldnt find a free block\n");
        return SYSERR;
      }
      printf("new cur_block: %d\n", cur_block);
      // cur_block_left = fsd.blocksz;
      // printf("Current mask bit %d\n", fs_getmaskbit(cur_block));
      // fs_printfreemask();
      fs_setmaskbit(cur_block);
    } 
    

    //calculate current length to be written
    if(len < fsd.blocksz - cur_offset) {
      cur_len = len;
    } else {
      cur_len = fsd.blocksz - cur_offset;
    }
    
    memcpy(block_cache, buf+total_write_len, cur_len);
    //Write to the block
    bs_bwrite(dev0, cur_block, cur_offset, block_cache, cur_len);

    total_write_len=total_write_len+cur_len;
    //Update Inode table
    
    
    if(cur_offset == 0) { // This means its a new block so updating the block entry
      ftptr->in.blocks[ftptr->in.size / fsd.blocksz] = cur_block;
      printf("Updating block entry: %d\n", ftptr->in.blocks[ftptr->in.size / fsd.blocksz]);
    }
    ftptr->in.size = ftptr->in.size + cur_len;
    printf("Writing size: %d\n",ftptr->in.size);

    //Update Filetable
    ftptr->fileptr=ftptr->fileptr+cur_len;
    len = len - cur_len;

    printf("total_write_len: %d\n", total_write_len);
    }

    printf("Updating Inode at: %d\n", ftptr->in.id);
    fs_put_inode_by_num(dev0, ftptr->in.id, &ftptr->in);   
    return total_write_len;
}

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock) {
  int diskblock;

  if (fileblock >= INODEBLOCKS - 2) {
    printf("No indirect block support\n");
    return SYSERR;
  }

  diskblock = oft[fd].in.blocks[fileblock]; //get the logical block address

  return diskblock;
}

/* read in an inode and fill in the pointer */
int
fs_get_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;
  int inode_off;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_get_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  inode_off = inn * sizeof(struct inode);

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  printf("inn*sizeof(struct inode): %d\n", inode_off);
  */

  bs_bread(dev0, bl, 0, &block_cache[0], fsd.blocksz);
  memcpy(in, &block_cache[inode_off], sizeof(struct inode));

  return OK;

}

int
fs_put_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_put_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  */

  bs_bread(dev0, bl, 0, block_cache, fsd.blocksz);
  memcpy(&block_cache[(inn*sizeof(struct inode))], in, sizeof(struct inode));
  bs_bwrite(dev0, bl, 0, block_cache, fsd.blocksz);

  return OK;
}
     
int fs_mkfs(int dev, int num_inodes) {
  int i;
  
  if (dev == 0) {
    fsd.nblocks = dev0_numblocks;
    fsd.blocksz = dev0_blocksize;
  }
  else {
    printf("Unsupported device\n");
    return SYSERR;
  }

  if (num_inodes < 1) {
    fsd.ninodes = DEFAULT_NUM_INODES;
  }
  else {
    fsd.ninodes = num_inodes;
  }

  i = fsd.nblocks;
  while ( (i % 8) != 0) {i++;}
  fsd.freemaskbytes = i / 8; 
  
  if ((fsd.freemask = getmem(fsd.freemaskbytes)) == (void *)SYSERR) {
    printf("fs_mkfs memget failed.\n");
    return SYSERR;
  }
  
  /* zero the free mask */
  for(i=0;i<fsd.freemaskbytes;i++) {
    fsd.freemask[i] = '\0';
  }
  
  fsd.inodes_used = 0;
  
  /* write the fsystem block to SB_BLK, mark block used */
  fs_setmaskbit(SB_BLK);
  bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
  
  /* write the free block bitmask in BM_BLK, mark block used */
  fs_setmaskbit(BM_BLK);
  bs_bwrite(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);

  return 1;
}

void
fs_print_fsd(void) {

  printf("fsd.ninodes: %d\n", fsd.ninodes);
  printf("sizeof(struct inode): %d\n", sizeof(struct inode));
  printf("INODES_PER_BLOCK: %d\n", INODES_PER_BLOCK);
  printf("NUM_INODE_BLOCKS: %d\n", NUM_INODE_BLOCKS);
}


/* This is maybe a little overcomplicated since the lowest-numbered
   block is indicated in the high-order bit.  Shift the byte by j
   positions to make the match in bit7 (the 8th bit) and then shift
   that value 7 times to the low-order bit to print.  Yes, it could be
   the other way...  */
void fs_printfreemask(void) {
  int i,j;

  for (i=0; i < fsd.freemaskbytes; i++) {
    for (j=0; j < 8; j++) {
      printf("%d", ((fsd.freemask[i] << j) & 0x80) >> 7);
    }
    if ( (i % 8) == 7) {
      printf("\n");
    }
  }
  printf("\n");
}