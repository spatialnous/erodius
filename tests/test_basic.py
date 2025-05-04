# SPDX-FileCopyrightText: 2025 Petros Koutsolampros
#
# SPDX-License-Identifier: GPL-3.0-only

from __future__ import annotations
import os

import erodius as m

def test_version():
    assert m.__version__ == "0.0.1"

def test_readMetaGraph():
    graphFile = os.path.join(os.path.dirname(__file__), 'data', 'barnsburySmall.graph')
    mgd = m.readMetaGraph(graphFile, False)
    assert mgd['shapeMaps'][0]['map'].get_name() == "/tmp/Rtmp3CVtFg/file408014dde327b.tsv"