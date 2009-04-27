#ifndef VMFS_VOLUME_H
#define VMFS_VOLUME_H

/* === Volume Info === */
#define VMFS_VOLINFO_BASE   0x100000
#define VMFS_VOLINFO_MAGIC  0xc001d00d

#define VMFS_VOLINFO_OFS_MAGIC 0x0000
#define VMFS_VOLINFO_OFS_VER   0x0004
#define VMFS_VOLINFO_OFS_LUN   0x000e
#define VMFS_VOLINFO_OFS_NAME  0x0012
#define VMFS_VOLINFO_OFS_UUID  0x0082 
/* 0x0092 64-bits timestamp in usec (volume ctime?) */
/* 0x009a 64-bits timestamp in usec (volume mtime?) */

#define VMFS_VOLINFO_OFS_NAME_SIZE     28

/* === LVM Info === */
#define VMFS_LVMINFO_OFS_SIZE          0x0200
#define VMFS_LVMINFO_OFS_BLKS          0x0208 /* Seems to be systematically sum(VMFS_LVMINFO_OFS_NUM_SEGMENTS for all extents) + VMFS_LVMINFO_OFS_NUM_EXTENTS */
#define VMFS_LVMINFO_OFS_UUID_STR      0x0214
#define VMFS_LVMINFO_OFS_UUID          0x0254
/* 0x0268 64-bits timestamp in usec (lvm ctime?) */
#define VMFS_LVMINFO_OFS_NUM_SEGMENTS  0x0274
#define VMFS_LVMINFO_OFS_FIRST_SEGMENT 0x0278
#define VMFS_LVMINFO_OFS_LAST_SEGMENT  0x0280
/* 0x0288 64-bits timestamp in usec (lvm mtime?) */
#define VMFS_LVMINFO_OFS_NUM_EXTENTS   0x0290

/* Segment information are at 0x80600 + i * 0x80 for i between 0 and VMFS_LVMINFO_OFS_NUM_SEGMENTS */
/* At 0x10 (64-bits) or 0x14 (32-bits) within a segment info, it seems like something related to the absolute segment number in the logical volume (looks like absolute segment number << 4 on 32-bits) */
/* Other segment information seem relative to the extent (always the same pattern on all extents) */

struct vmfs_volinfo {
   m_u32_t magic;
   m_u32_t version;
   char *name;
   uuid_t uuid;
   int lun;

   m_u64_t size;
   m_u64_t blocks;
   uuid_t lvm_uuid;
   m_u32_t num_segments,
           first_segment,
           last_segment,
           num_extents;
};

/* === VMFS mounted-volume === */
struct vmfs_volume {
   char *filename;
   int fd;
   vmfs_flags_t flags;
   int is_blkdev;
   int scsi_reservation;

   /* VMFS volume base */
   off_t vmfs_base;

   /* Volume and FS information */
   vmfs_volinfo_t vol_info;
};

/* Read a raw block of data on logical volume */
ssize_t vmfs_vol_read(const vmfs_volume_t *vol,off_t pos,u_char *buf,size_t len);

/* Write a raw block of data on logical volume */
ssize_t vmfs_vol_write(const vmfs_volume_t *vol,off_t pos,const u_char *buf,size_t len);

/* Volume reservation */
int vmfs_vol_reserve(const vmfs_volume_t *vol);

/* Volume release */
int vmfs_vol_release(const vmfs_volume_t *vol);

/* Check if physical volume support reservation */
int vmfs_vol_check_reservation(vmfs_volume_t *vol);

/* Show volume information */
void vmfs_vol_show(const vmfs_volume_t *vol);

/* Create a volume structure */
vmfs_volume_t *vmfs_vol_create(const char *filename,vmfs_flags_t flags);

/* Open a VMFS volume */
int vmfs_vol_open(vmfs_volume_t *vol);

#endif
