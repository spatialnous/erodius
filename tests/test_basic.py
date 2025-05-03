# SPDX-FileCopyrightText: 2025 Petros Koutsolampros
#
# SPDX-License-Identifier: GPL-3.0-only

from __future__ import annotations

import erodius as m

def test_version():
    assert m.__version__ == "0.0.1"


def test_readMetaGraph():
    mgd = m.readMetaGraph("./tests/data/barnsburySmall.graph", False)
    assert mgd[0][0].get_name() == "/tmp/Rtmp3CVtFg/file408014dde327b.tsv"

# print(m.readMetaGraph("/home/petros/Projects/Current/2025/erodius/code/erodius/tests/data/barnsburySmall.graph", True))
dt = m.readMetaGraph("./tests/data/barnsburySmall.graph", True)
print(dt[0][0].get_name())