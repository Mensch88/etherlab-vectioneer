# IgH EtherCAT Master — Vectioneer fork, mirrored

This is a faithful, full-history mirror of the EtherCAT master fork maintained by
**[Vectioneer](https://www.vectioneer.com/)** at

> **<https://git.vectioneer.com/pub/etherlab>** — branch `stable/vectioneer`

**Their repository is the authoritative one.** This mirror is *not* affiliated with,
endorsed by, or supported by Vectioneer. Do not report problems to them through this
mirror, and do not assume anything here has their review.

## Credit

The history is mirrored in full — 2590 commits, 34 distinct author names, every one
keeping its original author, date and message. Nothing was squashed, rewritten or
re-attributed. That intact history, not this file, is the real record.

### How attribution works in this tree

Two mechanisms are in use, and you need both to see who wrote what.

**The git `author` field.** By that measure:

| Author | | |
|---|---:|---|
| Florian Pose, Ingenieurgemeinschaft IgH | 2240 | the original IgH EtherCAT Master |
| **Mark Verrijt, Vectioneer** | **106** | **this fork, 2021–2026** |
| Gavin Lambert, TOMRA | 59 | the unofficial patchset — see [`README-lambert.md`](README-lambert.md) |
| Patrick Bruenn | 27 | |
| Martin Troxler | 24 | |
| Graeme Foot | 17 | |
| Knud Baastrup | 16 | |
| Richard Hacker | 10 | |

…and 26 others; `git shortlog -sn stable/vectioneer` gives the full list.

**Credit in the commit message.** For most of this project's life — the Mercurial era,
and the patchset — a contributor sent a patch and the maintainer applied it, naming them
in the subject. The `author` field then records who *applied* it. **58 commits on
`stable/vectioneer` credit someone this way**, and `git shortlog` sees none of them:

```
git log --format='%h %s' stable/vectioneer \
  | grep -iE 'thanks to|patch(es)? (from|by)|contributed by'
```

That list includes Frank Heckenbach's frame-corruption fix (`765b9ea8`), Beckhoff's CCAT
patches (`cf773ecf`, `8e0fab9d`), R. Roesch's FoE fixes (`994cb9a1`, `0af9fa30`),
Patrick Bruenn on `ecdev_open()` (`7cb12f0c`), and the three below.

The three are described at length because this mirror's owner wrote them, and the diffs
were read while setting up this repository — not because they outweigh the rest of that
list:

| | |
|---|---|
| `8b5f700d` *Distributed Clock fixes from Jun Yuan* | The `app_time_sent` correction. The master had been computing the DC system-time offset against a `jiffies`-corrected application time rather than the time the read datagram actually went on the wire, so the reference slave started roughly one cycle out of lock. Also carried in the patchset as `base/0002-junyuan-dc_sync_issues.patch`, and merged into the official IgH tree as `17eddce6` |
| `170110f7`, `10ef2c54` *Applied ethtool patch from Jun Yuan* | Corrects which `e1000e` ethtool operations are refused while EtherCAT owns the NIC: adds the `adapter->ecdev` guard to `e1000_set_rx_csum()` and `e1000_nway_reset()`, which touch hardware, and drops it from `e1000_set_tx_csum()`, which only sets a feature flag |
| `4a858fc9` *Fixed compiler error in master.c; thanks to Jun Yuan* | Larger than its subject suggests. `ecrt_master_sdo_download_complete()` held its `ec_master_sdo_request_t` on the stack, but the request is scheduled asynchronously and released by the master through `kref_put()` — so it outlived its frame. Reworked to `kmalloc` + `kref_init`, with every error path releasing through the refcount |

The patchset's own file naming is the clearest record of all — `0001-graemef-…`,
`0002-junyuan-…`, `0003-frank-…`, `0004-gavinl-…`, one contributor's name per patch. See
the `patches` branch.

### Vectioneer's own work

Not only volume. Since 2021 it includes the RPS application-cycle synchronization that
keeps the master's background threads out of the application's send window, and the
replacement of the per-slave mailbox `rt_mutex` with a lock-free `atomic_cmpxchg` on
`read_mbox_busy`. They also carry the SII override and the distributed-clock fix above,
neither of which official `stable-1.6` has.

## Why this mirror exists

`git.vectioneer.com` publishes the source but has no public contribution path:
registration is closed, so an outside user cannot fork it, open an issue, or submit a
merge request there. This mirror is somewhere to publish patches written on top of their
work, and to keep a copy should that server go away.

To reach Vectioneer directly, email is the only channel.

## Branch layout

| Branch | Contents |
|---|---|
| `stable/vectioneer`, `develop/vectioneer` | **pristine**, exactly as published upstream — never modified here |
| `original/stable-1.5`, `original/stable-1.6`, `original/devel`, `patches`, `default` | upstream's own tracking branches, likewise untouched |
| `cosrobe/main` | our branch — this README, and any patches we write |

So our delta is always exactly:

```
git diff stable/vectioneer..cosrobe/main
```

Right now that diff is this file and nothing else.

To re-sync with upstream:

```
git remote add vectioneer https://git.vectioneer.com/pub/etherlab.git
git fetch vectioneer
git push origin 'refs/remotes/vectioneer/stable/vectioneer:refs/heads/stable/vectioneer'
```

## Where to send patches

Depends on what the patch touches:

* **Generic** — driver support, kernel compatibility, distributed clocks: send it to the
  **official IgH EtherCAT Master**, <https://gitlab.com/etherlab.org/ethercat>. That
  project is active (merge requests merging most weeks, `Stable 1.7` in draft) and is
  where a fix reaches the most users. Note that 1.6/1.7 has diverged from the
  1.5.2 + patchset line this tree follows, so a patch written here will not always apply
  there unmodified.
* **Specific to this fork** — anything touching the patchset's mailbox/SDO rework, SII
  override, or Vectioneer's own changes: email Vectioneer.
* Historic discussion of the patchset lives on the
  [etherlab-dev mailing list](https://lists.etherlab.org/mailman/listinfo/etherlab-dev).

## License

Unchanged from upstream — see [`COPYING`](COPYING) and [`COPYING.LESSER`](COPYING.LESSER).
Redistribution in this form is what those licenses provide for; the attribution above and
the intact history are how this mirror meets them.

Use of the EtherCAT technology and brand remains subject to the industrial property rights
of Beckhoff Automation GmbH.
