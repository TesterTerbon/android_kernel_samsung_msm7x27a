#include <linux/fanotify.h>
#include <linux/fdtable.h>
#include <linux/fsnotify_backend.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/kernel.h> /* UINT_MAX */
#include <linux/mount.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/wait.h>

<<<<<<< HEAD
static bool should_merge(struct fsnotify_event *old, struct fsnotify_event *new)
{
	pr_debug("%s: old=%p new=%p\n", __func__, old, new);

	if (old->to_tell == new->to_tell &&
	    old->data_type == new->data_type &&
	    old->tgid == new->tgid) {
		switch (old->data_type) {
		case (FSNOTIFY_EVENT_PATH):
#ifdef CONFIG_FANOTIFY_ACCESS_PERMISSIONS
			/* dont merge two permission events */
			if ((old->mask & FAN_ALL_PERM_EVENTS) &&
			    (new->mask & FAN_ALL_PERM_EVENTS))
				return false;
#endif
			if ((old->path.mnt == new->path.mnt) &&
			    (old->path.dentry == new->path.dentry))
				return true;
			break;
		case (FSNOTIFY_EVENT_NONE):
			return true;
		default:
			BUG();
		};
	}
=======
#include "fanotify.h"

static bool should_merge(struct fsnotify_event *old_fsn,
			 struct fsnotify_event *new_fsn)
{
	struct fanotify_event_info *old, *new;

#ifdef CONFIG_FANOTIFY_ACCESS_PERMISSIONS
	/* dont merge two permission events */
	if ((old_fsn->mask & FAN_ALL_PERM_EVENTS) &&
	    (new_fsn->mask & FAN_ALL_PERM_EVENTS))
		return false;
#endif
	pr_debug("%s: old=%p new=%p\n", __func__, old_fsn, new_fsn);
	old = FANOTIFY_E(old_fsn);
	new = FANOTIFY_E(new_fsn);

	if (old_fsn->inode == new_fsn->inode && old->tgid == new->tgid &&
	    old->path.mnt == new->path.mnt &&
	    old->path.dentry == new->path.dentry)
		return true;
>>>>>>> refs/remotes/origin/master
	return false;
}

/* and the list better be locked by something too! */
static struct fsnotify_event *fanotify_merge(struct list_head *list,
					     struct fsnotify_event *event)
{
<<<<<<< HEAD
	struct fsnotify_event_holder *test_holder;
	struct fsnotify_event *test_event = NULL;
	struct fsnotify_event *new_event;

	pr_debug("%s: list=%p event=%p\n", __func__, list, event);


	list_for_each_entry_reverse(test_holder, list, event_list) {
		if (should_merge(test_holder->event, event)) {
			test_event = test_holder->event;
=======
	struct fsnotify_event *test_event;
	bool do_merge = false;

	pr_debug("%s: list=%p event=%p\n", __func__, list, event);

	list_for_each_entry_reverse(test_event, list, list) {
		if (should_merge(test_event, event)) {
			do_merge = true;
>>>>>>> refs/remotes/origin/master
			break;
		}
	}

<<<<<<< HEAD
	if (!test_event)
		return NULL;

	fsnotify_get_event(test_event);

	/* if they are exactly the same we are done */
	if (test_event->mask == event->mask)
		return test_event;

	/*
	 * if the refcnt == 2 this is the only queue
	 * for this event and so we can update the mask
	 * in place.
	 */
	if (atomic_read(&test_event->refcnt) == 2) {
		test_event->mask |= event->mask;
		return test_event;
	}

	new_event = fsnotify_clone_event(test_event);

	/* done with test_event */
	fsnotify_put_event(test_event);

	/* couldn't allocate memory, merge was not possible */
	if (unlikely(!new_event))
		return ERR_PTR(-ENOMEM);

	/* build new event and replace it on the list */
	new_event->mask = (test_event->mask | event->mask);
	fsnotify_replace_event(test_holder, new_event);

	/* we hold a reference on new_event from clone_event */
	return new_event;
=======
	if (!do_merge)
		return NULL;

	test_event->mask |= event->mask;
	return test_event;
>>>>>>> refs/remotes/origin/master
}

#ifdef CONFIG_FANOTIFY_ACCESS_PERMISSIONS
static int fanotify_get_response_from_access(struct fsnotify_group *group,
<<<<<<< HEAD
					     struct fsnotify_event *event)
=======
					     struct fanotify_event_info *event)
>>>>>>> refs/remotes/origin/master
{
	int ret;

	pr_debug("%s: group=%p event=%p\n", __func__, group, event);

	wait_event(group->fanotify_data.access_waitq, event->response ||
				atomic_read(&group->fanotify_data.bypass_perm));

	if (!event->response) /* bypass_perm set */
		return 0;

	/* userspace responded, convert to something usable */
<<<<<<< HEAD
	spin_lock(&event->lock);
=======
>>>>>>> refs/remotes/origin/master
	switch (event->response) {
	case FAN_ALLOW:
		ret = 0;
		break;
	case FAN_DENY:
	default:
		ret = -EPERM;
	}
	event->response = 0;
<<<<<<< HEAD
	spin_unlock(&event->lock);
=======
>>>>>>> refs/remotes/origin/master

	pr_debug("%s: group=%p event=%p about to return ret=%d\n", __func__,
		 group, event, ret);
	
	return ret;
}
#endif

<<<<<<< HEAD
static int fanotify_handle_event(struct fsnotify_group *group,
				 struct fsnotify_mark *inode_mark,
				 struct fsnotify_mark *fanotify_mark,
				 struct fsnotify_event *event)
{
	int ret = 0;
	struct fsnotify_event *notify_event = NULL;

	BUILD_BUG_ON(FAN_ACCESS != FS_ACCESS);
	BUILD_BUG_ON(FAN_MODIFY != FS_MODIFY);
	BUILD_BUG_ON(FAN_CLOSE_NOWRITE != FS_CLOSE_NOWRITE);
	BUILD_BUG_ON(FAN_CLOSE_WRITE != FS_CLOSE_WRITE);
	BUILD_BUG_ON(FAN_OPEN != FS_OPEN);
	BUILD_BUG_ON(FAN_EVENT_ON_CHILD != FS_EVENT_ON_CHILD);
	BUILD_BUG_ON(FAN_Q_OVERFLOW != FS_Q_OVERFLOW);
	BUILD_BUG_ON(FAN_OPEN_PERM != FS_OPEN_PERM);
	BUILD_BUG_ON(FAN_ACCESS_PERM != FS_ACCESS_PERM);
	BUILD_BUG_ON(FAN_ONDIR != FS_ISDIR);

	pr_debug("%s: group=%p event=%p\n", __func__, group, event);

	notify_event = fsnotify_add_notify_event(group, event, NULL, fanotify_merge);
	if (IS_ERR(notify_event))
		return PTR_ERR(notify_event);

#ifdef CONFIG_FANOTIFY_ACCESS_PERMISSIONS
	if (event->mask & FAN_ALL_PERM_EVENTS) {
		/* if we merged we need to wait on the new event */
		if (notify_event)
			event = notify_event;
		ret = fanotify_get_response_from_access(group, event);
	}
#endif

	if (notify_event)
		fsnotify_put_event(notify_event);

	return ret;
}

static bool fanotify_should_send_event(struct fsnotify_group *group,
				       struct inode *to_tell,
				       struct fsnotify_mark *inode_mark,
				       struct fsnotify_mark *vfsmnt_mark,
				       __u32 event_mask, void *data, int data_type)
=======
static bool fanotify_should_send_event(struct fsnotify_mark *inode_mark,
				       struct fsnotify_mark *vfsmnt_mark,
				       u32 event_mask,
				       void *data, int data_type)
>>>>>>> refs/remotes/origin/master
{
	__u32 marks_mask, marks_ignored_mask;
	struct path *path = data;

<<<<<<< HEAD
	pr_debug("%s: group=%p to_tell=%p inode_mark=%p vfsmnt_mark=%p "
		 "mask=%x data=%p data_type=%d\n", __func__, group, to_tell,
		 inode_mark, vfsmnt_mark, event_mask, data, data_type);
=======
	pr_debug("%s: inode_mark=%p vfsmnt_mark=%p mask=%x data=%p"
		 " data_type=%d\n", __func__, inode_mark, vfsmnt_mark,
		 event_mask, data, data_type);
>>>>>>> refs/remotes/origin/master

	/* if we don't have enough info to send an event to userspace say no */
	if (data_type != FSNOTIFY_EVENT_PATH)
		return false;

	/* sorry, fanotify only gives a damn about files and dirs */
	if (!S_ISREG(path->dentry->d_inode->i_mode) &&
	    !S_ISDIR(path->dentry->d_inode->i_mode))
		return false;

	if (inode_mark && vfsmnt_mark) {
		marks_mask = (vfsmnt_mark->mask | inode_mark->mask);
		marks_ignored_mask = (vfsmnt_mark->ignored_mask | inode_mark->ignored_mask);
	} else if (inode_mark) {
		/*
		 * if the event is for a child and this inode doesn't care about
		 * events on the child, don't send it!
		 */
		if ((event_mask & FS_EVENT_ON_CHILD) &&
		    !(inode_mark->mask & FS_EVENT_ON_CHILD))
			return false;
		marks_mask = inode_mark->mask;
		marks_ignored_mask = inode_mark->ignored_mask;
	} else if (vfsmnt_mark) {
		marks_mask = vfsmnt_mark->mask;
		marks_ignored_mask = vfsmnt_mark->ignored_mask;
	} else {
		BUG();
	}

	if (S_ISDIR(path->dentry->d_inode->i_mode) &&
	    (marks_ignored_mask & FS_ISDIR))
		return false;

	if (event_mask & marks_mask & ~marks_ignored_mask)
		return true;

	return false;
}

<<<<<<< HEAD
=======
static int fanotify_handle_event(struct fsnotify_group *group,
				 struct inode *inode,
				 struct fsnotify_mark *inode_mark,
				 struct fsnotify_mark *fanotify_mark,
				 u32 mask, void *data, int data_type,
				 const unsigned char *file_name)
{
	int ret = 0;
	struct fanotify_event_info *event;
	struct fsnotify_event *fsn_event;
	struct fsnotify_event *notify_fsn_event;

	BUILD_BUG_ON(FAN_ACCESS != FS_ACCESS);
	BUILD_BUG_ON(FAN_MODIFY != FS_MODIFY);
	BUILD_BUG_ON(FAN_CLOSE_NOWRITE != FS_CLOSE_NOWRITE);
	BUILD_BUG_ON(FAN_CLOSE_WRITE != FS_CLOSE_WRITE);
	BUILD_BUG_ON(FAN_OPEN != FS_OPEN);
	BUILD_BUG_ON(FAN_EVENT_ON_CHILD != FS_EVENT_ON_CHILD);
	BUILD_BUG_ON(FAN_Q_OVERFLOW != FS_Q_OVERFLOW);
	BUILD_BUG_ON(FAN_OPEN_PERM != FS_OPEN_PERM);
	BUILD_BUG_ON(FAN_ACCESS_PERM != FS_ACCESS_PERM);
	BUILD_BUG_ON(FAN_ONDIR != FS_ISDIR);

	if (!fanotify_should_send_event(inode_mark, fanotify_mark, mask, data,
					data_type))
		return 0;

	pr_debug("%s: group=%p inode=%p mask=%x\n", __func__, group, inode,
		 mask);

	event = kmem_cache_alloc(fanotify_event_cachep, GFP_KERNEL);
	if (unlikely(!event))
		return -ENOMEM;

	fsn_event = &event->fse;
	fsnotify_init_event(fsn_event, inode, mask);
	event->tgid = get_pid(task_tgid(current));
	if (data_type == FSNOTIFY_EVENT_PATH) {
		struct path *path = data;
		event->path = *path;
		path_get(&event->path);
	} else {
		event->path.mnt = NULL;
		event->path.dentry = NULL;
	}
#ifdef CONFIG_FANOTIFY_ACCESS_PERMISSIONS
	event->response = 0;
#endif

	notify_fsn_event = fsnotify_add_notify_event(group, fsn_event,
						     fanotify_merge);
	if (notify_fsn_event) {
		/* Our event wasn't used in the end. Free it. */
		fsnotify_destroy_event(group, fsn_event);
		if (IS_ERR(notify_fsn_event))
			return PTR_ERR(notify_fsn_event);
		/* We need to ask about a different events after a merge... */
		event = FANOTIFY_E(notify_fsn_event);
		fsn_event = notify_fsn_event;
	}

#ifdef CONFIG_FANOTIFY_ACCESS_PERMISSIONS
	if (fsn_event->mask & FAN_ALL_PERM_EVENTS)
		ret = fanotify_get_response_from_access(group, event);
#endif
	return ret;
}

>>>>>>> refs/remotes/origin/master
static void fanotify_free_group_priv(struct fsnotify_group *group)
{
	struct user_struct *user;

	user = group->fanotify_data.user;
	atomic_dec(&user->fanotify_listeners);
	free_uid(user);
}

<<<<<<< HEAD
const struct fsnotify_ops fanotify_fsnotify_ops = {
	.handle_event = fanotify_handle_event,
	.should_send_event = fanotify_should_send_event,
	.free_group_priv = fanotify_free_group_priv,
	.free_event_priv = NULL,
	.freeing_mark = NULL,
=======
static void fanotify_free_event(struct fsnotify_event *fsn_event)
{
	struct fanotify_event_info *event;

	event = FANOTIFY_E(fsn_event);
	path_put(&event->path);
	put_pid(event->tgid);
	kmem_cache_free(fanotify_event_cachep, event);
}

const struct fsnotify_ops fanotify_fsnotify_ops = {
	.handle_event = fanotify_handle_event,
	.free_group_priv = fanotify_free_group_priv,
	.free_event = fanotify_free_event,
>>>>>>> refs/remotes/origin/master
};
