#!/usr/bin/env python3
"""Run a command with deterministic environment-variable overrides."""

from __future__ import annotations

import argparse
import os
import subprocess
import sys


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--env", action="append", default=[], metavar="NAME=VALUE")
    parser.add_argument("command", nargs=argparse.REMAINDER)
    args = parser.parse_args()

    command = list(args.command)
    if command and command[0] == "--":
        command.pop(0)
    if not command:
        parser.error("missing command after --")

    env = os.environ.copy()
    for assignment in args.env:
        name, separator, value = assignment.partition("=")
        if not separator or not name:
            parser.error(f"invalid environment assignment: {assignment!r}")
        env[name] = value

    try:
        return subprocess.call(command, env=env)
    except FileNotFoundError as exc:
        print(f"error: command not found: {exc.filename}", file=sys.stderr)
        return 127


if __name__ == "__main__":
    raise SystemExit(main())

