/*
 *   fs/cifs/ioctl.c
 *
 *   vfs operations that deal with io control
 *
<<<<<<< HEAD
 *   Copyright (C) International Business Machines  Corp., 2005,2007
=======
 *   Copyright (C) International Business Machines  Corp., 2005,2013
>>>>>>> refs/remotes/origin/master
 *   Author(s): Steve French (sfrench@us.ibm.com)
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published
 *   by the Free Software Foundation; either version 2.1 of the License, or
 *   (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <linux/fs.h>
<<<<<<< HEAD
=======
#include <linux/file.h>
#include <linux/mount.h>
#include <linux/mm.h>
#include <linux/pagemap.h>
>>>>>>> refs/remotes/origin/master
#include "cifspdu.h"
#include "cifsglob.h"
#include "cifsproto.h"
#include "cifs_debug.h"
#include "cifsfs.h"

<<<<<<< HEAD
#define CIFS_IOC_CHECKUMOUNT _IO(0xCF, 2)

long cifs_ioctl(struct file *filep, unsigned int command, unsigned long arg)
{
	struct inode *inode = filep->f_dentry->d_inode;
	int rc = -ENOTTY; /* strange error - but the precedent */
	int xid;
	struct cifs_sb_info *cifs_sb;
#ifdef CONFIG_CIFS_POSIX
	struct cifsFileInfo *pSMBFile = filep->private_data;
	struct cifs_tcon *tcon;
	__u64	ExtAttrBits = 0;
	__u64	ExtAttrMask = 0;
	__u64   caps;
#endif /* CONFIG_CIFS_POSIX */

	xid = GetXid();

	cFYI(1, "ioctl file %p  cmd %u  arg %lu", filep, command, arg);
=======
#define CIFS_IOCTL_MAGIC	0xCF
#define CIFS_IOC_COPYCHUNK_FILE	_IOW(CIFS_IOCTL_MAGIC, 3, int)

static long cifs_ioctl_clone(unsigned int xid, struct file *dst_file,
			unsigned long srcfd, u64 off, u64 len, u64 destoff)
{
	int rc;
	struct cifsFileInfo *smb_file_target = dst_file->private_data;
	struct inode *target_inode = file_inode(dst_file);
	struct cifs_tcon *target_tcon;
	struct fd src_file;
	struct cifsFileInfo *smb_file_src;
	struct inode *src_inode;
	struct cifs_tcon *src_tcon;

	cifs_dbg(FYI, "ioctl clone range\n");
	/* the destination must be opened for writing */
	if (!(dst_file->f_mode & FMODE_WRITE)) {
		cifs_dbg(FYI, "file target not open for write\n");
		return -EINVAL;
	}

	/* check if target volume is readonly and take reference */
	rc = mnt_want_write_file(dst_file);
	if (rc) {
		cifs_dbg(FYI, "mnt_want_write failed with rc %d\n", rc);
		return rc;
	}

	src_file = fdget(srcfd);
	if (!src_file.file) {
		rc = -EBADF;
		goto out_drop_write;
	}

	if ((!src_file.file->private_data) || (!dst_file->private_data)) {
		rc = -EBADF;
		cifs_dbg(VFS, "missing cifsFileInfo on copy range src file\n");
		goto out_fput;
	}

	rc = -EXDEV;
	smb_file_target = dst_file->private_data;
	smb_file_src = src_file.file->private_data;
	src_tcon = tlink_tcon(smb_file_src->tlink);
	target_tcon = tlink_tcon(smb_file_target->tlink);

	/* check if source and target are on same tree connection */
	if (src_tcon != target_tcon) {
		cifs_dbg(VFS, "file copy src and target on different volume\n");
		goto out_fput;
	}

	src_inode = src_file.file->f_dentry->d_inode;

	/*
	 * Note: cifs case is easier than btrfs since server responsible for
	 * checks for proper open modes and file type and if it wants
	 * server could even support copy of range where source = target
	 */

	/* so we do not deadlock racing two ioctls on same files */
	if (target_inode < src_inode) {
		mutex_lock_nested(&target_inode->i_mutex, I_MUTEX_PARENT);
		mutex_lock_nested(&src_inode->i_mutex, I_MUTEX_CHILD);
	} else {
		mutex_lock_nested(&src_inode->i_mutex, I_MUTEX_PARENT);
		mutex_lock_nested(&target_inode->i_mutex, I_MUTEX_CHILD);
	}

	/* determine range to clone */
	rc = -EINVAL;
	if (off + len > src_inode->i_size || off + len < off)
		goto out_unlock;
	if (len == 0)
		len = src_inode->i_size - off;

	cifs_dbg(FYI, "about to flush pages\n");
	/* should we flush first and last page first */
	truncate_inode_pages_range(&target_inode->i_data, destoff,
				   PAGE_CACHE_ALIGN(destoff + len)-1);

	if (target_tcon->ses->server->ops->clone_range)
		rc = target_tcon->ses->server->ops->clone_range(xid,
			smb_file_src, smb_file_target, off, len, destoff);

	/* force revalidate of size and timestamps of target file now
	   that target is updated on the server */
	CIFS_I(target_inode)->time = 0;
out_unlock:
	/* although unlocking in the reverse order from locking is not
	   strictly necessary here it is a little cleaner to be consistent */
	if (target_inode < src_inode) {
		mutex_unlock(&src_inode->i_mutex);
		mutex_unlock(&target_inode->i_mutex);
	} else {
		mutex_unlock(&target_inode->i_mutex);
		mutex_unlock(&src_inode->i_mutex);
	}
out_fput:
	fdput(src_file);
out_drop_write:
	mnt_drop_write_file(dst_file);
	return rc;
}

long cifs_ioctl(struct file *filep, unsigned int command, unsigned long arg)
{
	struct inode *inode = file_inode(filep);
	int rc = -ENOTTY; /* strange error - but the precedent */
	unsigned int xid;
	struct cifs_sb_info *cifs_sb;
	struct cifsFileInfo *pSMBFile = filep->private_data;
	struct cifs_tcon *tcon;
	__u64	ExtAttrBits = 0;
	__u64   caps;

	xid = get_xid();

	cifs_dbg(FYI, "ioctl file %p  cmd %u  arg %lu\n", filep, command, arg);
>>>>>>> refs/remotes/origin/master

	cifs_sb = CIFS_SB(inode->i_sb);

	switch (command) {
<<<<<<< HEAD
		case CIFS_IOC_CHECKUMOUNT:
			cFYI(1, "User unmount attempted");
			if (cifs_sb->mnt_uid == current_uid())
				rc = 0;
			else {
				rc = -EACCES;
				cFYI(1, "uids do not match");
			}
			break;
#ifdef CONFIG_CIFS_POSIX
=======
>>>>>>> refs/remotes/origin/master
		case FS_IOC_GETFLAGS:
			if (pSMBFile == NULL)
				break;
			tcon = tlink_tcon(pSMBFile->tlink);
			caps = le64_to_cpu(tcon->fsUnixInfo.Capability);
<<<<<<< HEAD
			if (CIFS_UNIX_EXTATTR_CAP & caps) {
				rc = CIFSGetExtAttr(xid, tcon, pSMBFile->netfid,
					&ExtAttrBits, &ExtAttrMask);
=======
#ifdef CONFIG_CIFS_POSIX
			if (CIFS_UNIX_EXTATTR_CAP & caps) {
				__u64	ExtAttrMask = 0;
				rc = CIFSGetExtAttr(xid, tcon,
						    pSMBFile->fid.netfid,
						    &ExtAttrBits, &ExtAttrMask);
>>>>>>> refs/remotes/origin/master
				if (rc == 0)
					rc = put_user(ExtAttrBits &
						FS_FL_USER_VISIBLE,
						(int __user *)arg);
<<<<<<< HEAD
			}
			break;

=======
				if (rc != EOPNOTSUPP)
					break;
			}
#endif /* CONFIG_CIFS_POSIX */
			rc = 0;
			if (CIFS_I(inode)->cifsAttrs & ATTR_COMPRESSED) {
				/* add in the compressed bit */
				ExtAttrBits = FS_COMPR_FL;
				rc = put_user(ExtAttrBits & FS_FL_USER_VISIBLE,
					      (int __user *)arg);
			}
			break;
>>>>>>> refs/remotes/origin/master
		case FS_IOC_SETFLAGS:
			if (pSMBFile == NULL)
				break;
			tcon = tlink_tcon(pSMBFile->tlink);
			caps = le64_to_cpu(tcon->fsUnixInfo.Capability);
<<<<<<< HEAD
			if (CIFS_UNIX_EXTATTR_CAP & caps) {
				if (get_user(ExtAttrBits, (int __user *)arg)) {
					rc = -EFAULT;
					break;
				}
				/* rc= CIFSGetExtAttr(xid,tcon,pSMBFile->netfid,
					extAttrBits, &ExtAttrMask);*/
			}
			cFYI(1, "set flags not implemented yet");
			break;
#endif /* CONFIG_CIFS_POSIX */
		default:
			cFYI(1, "unsupported ioctl");
			break;
	}

	FreeXid(xid);
=======

			if (get_user(ExtAttrBits, (int __user *)arg)) {
				rc = -EFAULT;
				break;
			}

			/*
			 * if (CIFS_UNIX_EXTATTR_CAP & caps)
			 *	rc = CIFSSetExtAttr(xid, tcon,
			 *		       pSMBFile->fid.netfid,
			 *		       extAttrBits,
			 *		       &ExtAttrMask);
			 * if (rc != EOPNOTSUPP)
			 *	break;
			 */

			/* Currently only flag we can set is compressed flag */
			if ((ExtAttrBits & FS_COMPR_FL) == 0)
				break;

			/* Try to set compress flag */
			if (tcon->ses->server->ops->set_compression) {
				rc = tcon->ses->server->ops->set_compression(
							xid, tcon, pSMBFile);
				cifs_dbg(FYI, "set compress flag rc %d\n", rc);
			}
			break;
		case CIFS_IOC_COPYCHUNK_FILE:
			rc = cifs_ioctl_clone(xid, filep, arg, 0, 0, 0);
			break;
		default:
			cifs_dbg(FYI, "unsupported ioctl\n");
			break;
	}

	free_xid(xid);
>>>>>>> refs/remotes/origin/master
	return rc;
}
