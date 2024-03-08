resolved = [
     {
          "original_rule_class": "local_repository",
          "original_attributes": {
               "name": "bazel_tools",
               "path": "/home/ubuntu/.cache/bazel/_bazel_ubuntu/install/14fb027596f626f2526df4873ea20b8b/embedded_tools"
          },
          "native": "local_repository(name = \"bazel_tools\", path = __embedded_dir__ + \"/\" + \"embedded_tools\")"
     },
     {
          "original_rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
          "definition_information": "Repository rules_proto instantiated at:\n  /home/ubuntu/ParClusterers/WORKSPACE:67:13: in <toplevel>\nRepository rule http_archive defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/http.bzl:381:31: in <toplevel>\n",
          "original_attributes": {
               "name": "rules_proto",
               "urls": [
                    "https://mirror.bazel.build/github.com/bazelbuild/rules_proto/archive/refs/tags/4.0.0.tar.gz",
                    "https://github.com/bazelbuild/rules_proto/archive/refs/tags/4.0.0.tar.gz"
               ],
               "sha256": "66bfdf8782796239d3875d37e7de19b1d94301e8972b3cbd2446b332429b4df1",
               "strip_prefix": "rules_proto-4.0.0"
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
                    "attributes": {
                         "url": "",
                         "urls": [
                              "https://mirror.bazel.build/github.com/bazelbuild/rules_proto/archive/refs/tags/4.0.0.tar.gz",
                              "https://github.com/bazelbuild/rules_proto/archive/refs/tags/4.0.0.tar.gz"
                         ],
                         "sha256": "66bfdf8782796239d3875d37e7de19b1d94301e8972b3cbd2446b332429b4df1",
                         "integrity": "",
                         "netrc": "",
                         "auth_patterns": {},
                         "canonical_id": "",
                         "strip_prefix": "rules_proto-4.0.0",
                         "add_prefix": "",
                         "type": "",
                         "patches": [],
                         "remote_patches": {},
                         "remote_patch_strip": 0,
                         "patch_tool": "",
                         "patch_args": [
                              "-p0"
                         ],
                         "patch_cmds": [],
                         "patch_cmds_win": [],
                         "build_file_content": "",
                         "workspace_file_content": "",
                         "name": "rules_proto"
                    },
                    "output_tree_hash": "b023cb39c4ffadad4f3dfed69c5e2106c3867742f898fd631f33658c86523034"
               }
          ]
     },
     {
          "original_rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
          "definition_information": "Repository rules_cc instantiated at:\n  /home/ubuntu/ParClusterers/WORKSPACE:77:25: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_proto/proto/repositories.bzl:29:14: in rules_proto_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\nRepository rule http_archive defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/http.bzl:381:31: in <toplevel>\n",
          "original_attributes": {
               "name": "rules_cc",
               "generator_name": "rules_cc",
               "generator_function": "rules_proto_dependencies",
               "generator_location": None,
               "urls": [
                    "https://mirror.bazel.build/github.com/bazelbuild/rules_cc/archive/b7fe9697c0c76ab2fd431a891dbb9a6a32ed7c3e.tar.gz",
                    "https://github.com/bazelbuild/rules_cc/archive/b7fe9697c0c76ab2fd431a891dbb9a6a32ed7c3e.tar.gz"
               ],
               "sha256": "29daf0159f0cf552fcff60b49d8bcd4f08f08506d2da6e41b07058ec50cfeaec",
               "strip_prefix": "rules_cc-b7fe9697c0c76ab2fd431a891dbb9a6a32ed7c3e"
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
                    "attributes": {
                         "url": "",
                         "urls": [
                              "https://mirror.bazel.build/github.com/bazelbuild/rules_cc/archive/b7fe9697c0c76ab2fd431a891dbb9a6a32ed7c3e.tar.gz",
                              "https://github.com/bazelbuild/rules_cc/archive/b7fe9697c0c76ab2fd431a891dbb9a6a32ed7c3e.tar.gz"
                         ],
                         "sha256": "29daf0159f0cf552fcff60b49d8bcd4f08f08506d2da6e41b07058ec50cfeaec",
                         "integrity": "",
                         "netrc": "",
                         "auth_patterns": {},
                         "canonical_id": "",
                         "strip_prefix": "rules_cc-b7fe9697c0c76ab2fd431a891dbb9a6a32ed7c3e",
                         "add_prefix": "",
                         "type": "",
                         "patches": [],
                         "remote_patches": {},
                         "remote_patch_strip": 0,
                         "patch_tool": "",
                         "patch_args": [
                              "-p0"
                         ],
                         "patch_cmds": [],
                         "patch_cmds_win": [],
                         "build_file_content": "",
                         "workspace_file_content": "",
                         "name": "rules_cc"
                    },
                    "output_tree_hash": "610c6c9e5d42fe454d33d4c69d49cada7ff80dc14febf19020f8242bf2b8a487"
               }
          ]
     },
     {
          "original_rule_class": "local_repository",
          "original_attributes": {
               "name": "rules_java_builtin",
               "path": "/home/ubuntu/.cache/bazel/_bazel_ubuntu/install/14fb027596f626f2526df4873ea20b8b/rules_java"
          },
          "native": "local_repository(name = \"rules_java_builtin\", path = __embedded_dir__ + \"/\" + \"rules_java\")"
     },
     {
          "original_rule_class": "local_config_platform",
          "original_attributes": {
               "name": "local_config_platform"
          },
          "native": "local_config_platform(name = 'local_config_platform')"
     },
     {
          "original_rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
          "definition_information": "Repository platforms instantiated at:\n  /home/ubuntu/ParClusterers/WORKSPACE:155:13: in <toplevel>\nRepository rule http_archive defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/http.bzl:381:31: in <toplevel>\n",
          "original_attributes": {
               "name": "platforms",
               "urls": [
                    "https://github.com/bazelbuild/platforms/archive/98939346da932eef0b54cf808622f5bb0928f00b.zip"
               ],
               "sha256": "b601beaf841244de5c5a50d2b2eddd34839788000fa1be4260ce6603ca0d8eb7",
               "strip_prefix": "platforms-98939346da932eef0b54cf808622f5bb0928f00b"
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
                    "attributes": {
                         "url": "",
                         "urls": [
                              "https://github.com/bazelbuild/platforms/archive/98939346da932eef0b54cf808622f5bb0928f00b.zip"
                         ],
                         "sha256": "b601beaf841244de5c5a50d2b2eddd34839788000fa1be4260ce6603ca0d8eb7",
                         "integrity": "",
                         "netrc": "",
                         "auth_patterns": {},
                         "canonical_id": "",
                         "strip_prefix": "platforms-98939346da932eef0b54cf808622f5bb0928f00b",
                         "add_prefix": "",
                         "type": "",
                         "patches": [],
                         "remote_patches": {},
                         "remote_patch_strip": 0,
                         "patch_tool": "",
                         "patch_args": [
                              "-p0"
                         ],
                         "patch_cmds": [],
                         "patch_cmds_win": [],
                         "build_file_content": "",
                         "workspace_file_content": "",
                         "name": "platforms"
                    },
                    "output_tree_hash": "779b1f1d9d6c1e0dff20f7f7f4703ce6c209f431484c2847b4b63a5017c9abdf"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk11_linux_ppc64le_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:526:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:215:10: in remote_jdk11_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk11_linux_ppc64le_toolchain_config_repo",
               "generator_name": "remotejdk11_linux_ppc64le_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_11\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"11\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:ppc\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_linux_ppc64le//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:ppc\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_linux_ppc64le//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk11_linux_ppc64le_toolchain_config_repo",
                         "generator_name": "remotejdk11_linux_ppc64le_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_11\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"11\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:ppc\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_linux_ppc64le//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:ppc\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_linux_ppc64le//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "3272b586976beea589d09ea8029fd5d714da40127c8850e3480991c2440c5825"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk11_macos_aarch64_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:526:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:263:10: in remote_jdk11_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk11_macos_aarch64_toolchain_config_repo",
               "generator_name": "remotejdk11_macos_aarch64_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_11\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"11\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_macos_aarch64//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_macos_aarch64//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk11_macos_aarch64_toolchain_config_repo",
                         "generator_name": "remotejdk11_macos_aarch64_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_11\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"11\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_macos_aarch64//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_macos_aarch64//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "ca1d067909669aa58188026a7da06d43bdec74a3ba5c122af8a4c3660acd8d8f"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk11_macos_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:526:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:247:10: in remote_jdk11_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk11_macos_toolchain_config_repo",
               "generator_name": "remotejdk11_macos_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_11\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"11\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_macos//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_macos//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk11_macos_toolchain_config_repo",
                         "generator_name": "remotejdk11_macos_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_11\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"11\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_macos//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_macos//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "45b3b36d22d3e614745e7a5e838351c32fe0eabb09a4a197bac0f4d416a950ce"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk11_win_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:526:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:279:10: in remote_jdk11_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk11_win_toolchain_config_repo",
               "generator_name": "remotejdk11_win_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_11\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"11\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_win//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_win//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk11_win_toolchain_config_repo",
                         "generator_name": "remotejdk11_win_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_11\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"11\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_win//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_win//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "d0587a4ecc9323d5cf65314b2d284b520ffb5ee1d3231cc6601efa13dadcc0f4"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk11_linux_aarch64_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:526:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:199:10: in remote_jdk11_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk11_linux_aarch64_toolchain_config_repo",
               "generator_name": "remotejdk11_linux_aarch64_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_11\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"11\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_linux_aarch64//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_linux_aarch64//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk11_linux_aarch64_toolchain_config_repo",
                         "generator_name": "remotejdk11_linux_aarch64_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_11\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"11\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_linux_aarch64//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_linux_aarch64//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "bef508c068dd47d605f62c53ab0628f1f7f5101fdcc8ada09b2067b36c47931f"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk11_linux_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:526:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:183:10: in remote_jdk11_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk11_linux_toolchain_config_repo",
               "generator_name": "remotejdk11_linux_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_11\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"11\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_linux//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_linux//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk11_linux_toolchain_config_repo",
                         "generator_name": "remotejdk11_linux_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_11\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"11\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_linux//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_linux//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "0a170bf4f31e6c4621aeb4d4ce4b75b808be2f3a63cb55dc8172c27707d299ab"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:local_java_repository.bzl%_local_java_repository_rule",
          "definition_information": "Repository local_jdk instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:85:6: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/local_java_repository.bzl:315:32: in local_java_repository\nRepository rule _local_java_repository_rule defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/local_java_repository.bzl:270:46: in <toplevel>\n",
          "original_attributes": {
               "name": "local_jdk",
               "generator_name": "local_jdk",
               "generator_function": "maybe",
               "generator_location": None,
               "build_file_content": "load(\"@rules_java//java:defs.bzl\", \"java_runtime\")\n\npackage(default_visibility = [\"//visibility:public\"])\n\nexports_files([\"WORKSPACE\", \"BUILD.bazel\"])\n\nfilegroup(\n    name = \"jre\",\n    srcs = glob(\n        [\n            \"jre/bin/**\",\n            \"jre/lib/**\",\n        ],\n        allow_empty = True,\n        # In some configurations, Java browser plugin is considered harmful and\n        # common antivirus software blocks access to npjp2.dll interfering with Bazel,\n        # so do not include it in JRE on Windows.\n        exclude = [\"jre/bin/plugin2/**\"],\n    ),\n)\n\nfilegroup(\n    name = \"jdk-bin\",\n    srcs = glob(\n        [\"bin/**\"],\n        # The JDK on Windows sometimes contains a directory called\n        # \"%systemroot%\", which is not a valid label.\n        exclude = [\"**/*%*/**\"],\n    ),\n)\n\n# This folder holds security policies.\nfilegroup(\n    name = \"jdk-conf\",\n    srcs = glob(\n        [\"conf/**\"],\n        allow_empty = True,\n    ),\n)\n\nfilegroup(\n    name = \"jdk-include\",\n    srcs = glob(\n        [\"include/**\"],\n        allow_empty = True,\n    ),\n)\n\nfilegroup(\n    name = \"jdk-lib\",\n    srcs = glob(\n        [\"lib/**\", \"release\"],\n        allow_empty = True,\n        exclude = [\n            \"lib/missioncontrol/**\",\n            \"lib/visualvm/**\",\n        ],\n    ),\n)\n\njava_runtime(\n    name = \"jdk\",\n    srcs = [\n        \":jdk-bin\",\n        \":jdk-conf\",\n        \":jdk-include\",\n        \":jdk-lib\",\n        \":jre\",\n    ],\n    # Provide the 'java` binary explicitly so that the correct path is used by\n    # Bazel even when the host platform differs from the execution platform.\n    # Exactly one of the two globs will be empty depending on the host platform.\n    # When --incompatible_disallow_empty_glob is enabled, each individual empty\n    # glob will fail without allow_empty = True, even if the overall result is\n    # non-empty.\n    java = glob([\"bin/java.exe\", \"bin/java\"], allow_empty = True)[0],\n    version = {RUNTIME_VERSION},\n)\n",
               "java_home": "",
               "version": ""
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:local_java_repository.bzl%_local_java_repository_rule",
                    "attributes": {
                         "name": "local_jdk",
                         "generator_name": "local_jdk",
                         "generator_function": "maybe",
                         "generator_location": None,
                         "build_file_content": "load(\"@rules_java//java:defs.bzl\", \"java_runtime\")\n\npackage(default_visibility = [\"//visibility:public\"])\n\nexports_files([\"WORKSPACE\", \"BUILD.bazel\"])\n\nfilegroup(\n    name = \"jre\",\n    srcs = glob(\n        [\n            \"jre/bin/**\",\n            \"jre/lib/**\",\n        ],\n        allow_empty = True,\n        # In some configurations, Java browser plugin is considered harmful and\n        # common antivirus software blocks access to npjp2.dll interfering with Bazel,\n        # so do not include it in JRE on Windows.\n        exclude = [\"jre/bin/plugin2/**\"],\n    ),\n)\n\nfilegroup(\n    name = \"jdk-bin\",\n    srcs = glob(\n        [\"bin/**\"],\n        # The JDK on Windows sometimes contains a directory called\n        # \"%systemroot%\", which is not a valid label.\n        exclude = [\"**/*%*/**\"],\n    ),\n)\n\n# This folder holds security policies.\nfilegroup(\n    name = \"jdk-conf\",\n    srcs = glob(\n        [\"conf/**\"],\n        allow_empty = True,\n    ),\n)\n\nfilegroup(\n    name = \"jdk-include\",\n    srcs = glob(\n        [\"include/**\"],\n        allow_empty = True,\n    ),\n)\n\nfilegroup(\n    name = \"jdk-lib\",\n    srcs = glob(\n        [\"lib/**\", \"release\"],\n        allow_empty = True,\n        exclude = [\n            \"lib/missioncontrol/**\",\n            \"lib/visualvm/**\",\n        ],\n    ),\n)\n\njava_runtime(\n    name = \"jdk\",\n    srcs = [\n        \":jdk-bin\",\n        \":jdk-conf\",\n        \":jdk-include\",\n        \":jdk-lib\",\n        \":jre\",\n    ],\n    # Provide the 'java` binary explicitly so that the correct path is used by\n    # Bazel even when the host platform differs from the execution platform.\n    # Exactly one of the two globs will be empty depending on the host platform.\n    # When --incompatible_disallow_empty_glob is enabled, each individual empty\n    # glob will fail without allow_empty = True, even if the overall result is\n    # non-empty.\n    java = glob([\"bin/java.exe\", \"bin/java\"], allow_empty = True)[0],\n    version = {RUNTIME_VERSION},\n)\n",
                         "java_home": "",
                         "version": ""
                    },
                    "output_tree_hash": "c53c9cd5b92d02898c1cb8334ccbe84caebba8a7fbc3ffd6e42f689c93b64b13"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk11_win_arm64_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:526:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:295:10: in remote_jdk11_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk11_win_arm64_toolchain_config_repo",
               "generator_name": "remotejdk11_win_arm64_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_11\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"11\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:arm64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_win_arm64//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:arm64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_win_arm64//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk11_win_arm64_toolchain_config_repo",
                         "generator_name": "remotejdk11_win_arm64_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_11\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"11\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:arm64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_win_arm64//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:arm64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_win_arm64//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "c237bd9668de9b6437c452c020ea5bc717ff80b1a5ffd581adfdc7d4a6c5fe03"
               }
          ]
     },
     {
          "original_rule_class": "@@bazel_tools//tools/sh:sh_configure.bzl%sh_config",
          "definition_information": "Repository local_config_sh instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:174:13: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/sh/sh_configure.bzl:83:14: in sh_configure\nRepository rule sh_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/sh/sh_configure.bzl:72:28: in <toplevel>\n",
          "original_attributes": {
               "name": "local_config_sh",
               "generator_name": "local_config_sh",
               "generator_function": "sh_configure",
               "generator_location": None
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/sh:sh_configure.bzl%sh_config",
                    "attributes": {
                         "name": "local_config_sh",
                         "generator_name": "local_config_sh",
                         "generator_function": "sh_configure",
                         "generator_location": None
                    },
                    "output_tree_hash": "7bf5ba89669bcdf4526f821bc9f1f9f49409ae9c61c4e8f21c9f17e06c475127"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk17_linux_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:527:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:312:10: in remote_jdk17_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk17_linux_toolchain_config_repo",
               "generator_name": "remotejdk17_linux_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_17\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"17\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_linux//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_linux//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk17_linux_toolchain_config_repo",
                         "generator_name": "remotejdk17_linux_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_17\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"17\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_linux//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_linux//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "f0f07fe0f645f2dc7b8c9953c7962627e1c7425cc52f543729dbff16cd20e461"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk17_linux_aarch64_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:527:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:328:10: in remote_jdk17_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk17_linux_aarch64_toolchain_config_repo",
               "generator_name": "remotejdk17_linux_aarch64_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_17\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"17\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_linux_aarch64//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_linux_aarch64//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk17_linux_aarch64_toolchain_config_repo",
                         "generator_name": "remotejdk17_linux_aarch64_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_17\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"17\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_linux_aarch64//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_linux_aarch64//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "b169b01ac1a169d7eb5e3525454c3e408e9127993ac0f578dc2c5ad183fd4e3e"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk11_linux_s390x_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:526:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:231:10: in remote_jdk11_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk11_linux_s390x_toolchain_config_repo",
               "generator_name": "remotejdk11_linux_s390x_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_11\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"11\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:s390x\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_linux_s390x//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:s390x\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_linux_s390x//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk11_linux_s390x_toolchain_config_repo",
                         "generator_name": "remotejdk11_linux_s390x_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_11\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"11\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:s390x\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_linux_s390x//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:s390x\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk11_linux_s390x//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "244e11245106a8495ac4744a90023b87008e3e553766ba11d47a9fe5b4bb408d"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk17_linux_ppc64le_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:527:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:360:10: in remote_jdk17_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk17_linux_ppc64le_toolchain_config_repo",
               "generator_name": "remotejdk17_linux_ppc64le_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_17\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"17\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:ppc\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_linux_ppc64le//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:ppc\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_linux_ppc64le//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk17_linux_ppc64le_toolchain_config_repo",
                         "generator_name": "remotejdk17_linux_ppc64le_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_17\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"17\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:ppc\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_linux_ppc64le//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:ppc\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_linux_ppc64le//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "fdc8ae00f2436bfc46b2f54c84f2bd84122787ede232a4d61ffc284bfe6f61ec"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk17_macos_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:527:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:376:10: in remote_jdk17_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk17_macos_toolchain_config_repo",
               "generator_name": "remotejdk17_macos_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_17\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"17\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_macos//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_macos//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk17_macos_toolchain_config_repo",
                         "generator_name": "remotejdk17_macos_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_17\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"17\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_macos//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_macos//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "41aa7b3317f8d9001746e908454760bf544ffaa058abe22f40711246608022ba"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk17_macos_aarch64_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:527:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:392:10: in remote_jdk17_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk17_macos_aarch64_toolchain_config_repo",
               "generator_name": "remotejdk17_macos_aarch64_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_17\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"17\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_macos_aarch64//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_macos_aarch64//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk17_macos_aarch64_toolchain_config_repo",
                         "generator_name": "remotejdk17_macos_aarch64_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_17\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"17\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_macos_aarch64//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_macos_aarch64//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "0eb17f6d969bc665a21e55d29eb51e88a067159ee62cf5094b17658a07d3accb"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk17_win_arm64_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:527:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:422:10: in remote_jdk17_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk17_win_arm64_toolchain_config_repo",
               "generator_name": "remotejdk17_win_arm64_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_17\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"17\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:arm64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_win_arm64//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:arm64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_win_arm64//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk17_win_arm64_toolchain_config_repo",
                         "generator_name": "remotejdk17_win_arm64_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_17\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"17\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:arm64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_win_arm64//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:arm64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_win_arm64//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "86b129d9c464a9b08f97eca7d8bc5bdb3676b581f8aac044451dbdfaa49e69d3"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk21_linux_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:528:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:440:10: in remote_jdk21_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk21_linux_toolchain_config_repo",
               "generator_name": "remotejdk21_linux_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_21\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"21\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_linux//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_linux//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk21_linux_toolchain_config_repo",
                         "generator_name": "remotejdk21_linux_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_21\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"21\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_linux//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_linux//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "ee548ad054c9b75286ff3cd19792e433a2d1236378d3a0d8076fca0bb1a88e05"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk21_linux_aarch64_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:528:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:456:10: in remote_jdk21_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk21_linux_aarch64_toolchain_config_repo",
               "generator_name": "remotejdk21_linux_aarch64_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_21\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"21\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_linux_aarch64//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_linux_aarch64//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk21_linux_aarch64_toolchain_config_repo",
                         "generator_name": "remotejdk21_linux_aarch64_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_21\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"21\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_linux_aarch64//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_linux_aarch64//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "bb33021f243382d2fb849ec204c5c8be5083c37e081df71d34a84324687cf001"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk17_linux_s390x_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:527:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:344:10: in remote_jdk17_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk17_linux_s390x_toolchain_config_repo",
               "generator_name": "remotejdk17_linux_s390x_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_17\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"17\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:s390x\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_linux_s390x//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:s390x\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_linux_s390x//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk17_linux_s390x_toolchain_config_repo",
                         "generator_name": "remotejdk17_linux_s390x_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_17\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"17\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:s390x\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_linux_s390x//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:linux\", \"@platforms//cpu:s390x\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_linux_s390x//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "6ba1870e09fccfdcd423f4169b966a73f8e9deaff859ec6fb3b626ed61ebd8b5"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk17_win_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:527:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:407:10: in remote_jdk17_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk17_win_toolchain_config_repo",
               "generator_name": "remotejdk17_win_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_17\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"17\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_win//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_win//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk17_win_toolchain_config_repo",
                         "generator_name": "remotejdk17_win_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_17\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"17\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_win//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk17_win//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "170c3c9a35e502555dc9f04b345e064880acbf7df935f673154011356f4aad34"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk21_macos_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:528:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:472:10: in remote_jdk21_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk21_macos_toolchain_config_repo",
               "generator_name": "remotejdk21_macos_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_21\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"21\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_macos//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_macos//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk21_macos_toolchain_config_repo",
                         "generator_name": "remotejdk21_macos_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_21\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"21\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_macos//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_macos//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "434446eddb7f6a3dcc7a2a5330ed9ab26579c5142c19866b197475a695fbb32f"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk21_macos_aarch64_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:528:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:488:10: in remote_jdk21_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk21_macos_aarch64_toolchain_config_repo",
               "generator_name": "remotejdk21_macos_aarch64_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_21\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"21\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_macos_aarch64//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_macos_aarch64//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk21_macos_aarch64_toolchain_config_repo",
                         "generator_name": "remotejdk21_macos_aarch64_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_21\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"21\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_macos_aarch64//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:macos\", \"@platforms//cpu:aarch64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_macos_aarch64//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "706d910cc6809ea7f77fa4f938a4f019dd90d9dad927fb804a14b04321300a36"
               }
          ]
     },
     {
          "original_rule_class": "@@bazel_tools//tools/cpp:cc_configure.bzl%cc_autoconf_toolchains",
          "definition_information": "Repository local_config_cc_toolchains instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:168:13: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/cpp/cc_configure.bzl:148:27: in cc_configure\nRepository rule cc_autoconf_toolchains defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/cpp/cc_configure.bzl:47:41: in <toplevel>\n",
          "original_attributes": {
               "name": "local_config_cc_toolchains",
               "generator_name": "local_config_cc_toolchains",
               "generator_function": "cc_configure",
               "generator_location": None
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/cpp:cc_configure.bzl%cc_autoconf_toolchains",
                    "attributes": {
                         "name": "local_config_cc_toolchains",
                         "generator_name": "local_config_cc_toolchains",
                         "generator_function": "cc_configure",
                         "generator_location": None
                    },
                    "output_tree_hash": "f95f3d84ac75b4a4d9817af803f1d998a755bd9c20c700c79fa82cb159e98cfc"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
          "definition_information": "Repository remotejdk21_win_toolchain_config_repo instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:93:24: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:528:23: in rules_java_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/java/repositories.bzl:503:10: in remote_jdk21_repos\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:57:22: in remote_java_repository\nRepository rule _toolchain_config defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_java_builtin/toolchains/remote_java_repository.bzl:27:36: in <toplevel>\n",
          "original_attributes": {
               "name": "remotejdk21_win_toolchain_config_repo",
               "generator_name": "remotejdk21_win_toolchain_config_repo",
               "generator_function": "rules_java_dependencies",
               "generator_location": None,
               "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_21\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"21\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_win//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_win//:jdk\",\n)\n"
          },
          "repositories": [
               {
                    "rule_class": "@@rules_java_builtin//toolchains:remote_java_repository.bzl%_toolchain_config",
                    "attributes": {
                         "name": "remotejdk21_win_toolchain_config_repo",
                         "generator_name": "remotejdk21_win_toolchain_config_repo",
                         "generator_function": "rules_java_dependencies",
                         "generator_location": None,
                         "build_file": "\nconfig_setting(\n    name = \"prefix_version_setting\",\n    values = {\"java_runtime_version\": \"remotejdk_21\"},\n    visibility = [\"//visibility:private\"],\n)\nconfig_setting(\n    name = \"version_setting\",\n    values = {\"java_runtime_version\": \"21\"},\n    visibility = [\"//visibility:private\"],\n)\nalias(\n    name = \"version_or_prefix_version_setting\",\n    actual = select({\n        \":version_setting\": \":version_setting\",\n        \"//conditions:default\": \":prefix_version_setting\",\n    }),\n    visibility = [\"//visibility:private\"],\n)\ntoolchain(\n    name = \"toolchain\",\n    target_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_win//:jdk\",\n)\ntoolchain(\n    name = \"bootstrap_runtime_toolchain\",\n    # These constraints are not required for correctness, but prevent fetches of remote JDK for\n    # different architectures. As every Java compilation toolchain depends on a bootstrap runtime in\n    # the same configuration, this constraint will not result in toolchain resolution failures.\n    exec_compatible_with = [\"@platforms//os:windows\", \"@platforms//cpu:x86_64\"],\n    target_settings = [\":version_or_prefix_version_setting\"],\n    toolchain_type = \"@bazel_tools//tools/jdk:bootstrap_runtime_toolchain_type\",\n    toolchain = \"@remotejdk21_win//:jdk\",\n)\n"
                    },
                    "output_tree_hash": "87012328b07a779503deec0ef47132a0de50efd69afe7df87619bcc07b1dc4ed"
               }
          ]
     },
     {
          "original_rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
          "definition_information": "Repository rules_python instantiated at:\n  /home/ubuntu/ParClusterers/WORKSPACE:77:25: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_proto/proto/repositories.bzl:29:14: in rules_proto_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\nRepository rule http_archive defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/http.bzl:381:31: in <toplevel>\n",
          "original_attributes": {
               "name": "rules_python",
               "generator_name": "rules_python",
               "generator_function": "rules_proto_dependencies",
               "generator_location": None,
               "urls": [
                    "https://mirror.bazel.build/github.com/bazelbuild/rules_python/archive/4b84ad270387a7c439ebdccfd530e2339601ef27.tar.gz",
                    "https://github.com/bazelbuild/rules_python/archive/4b84ad270387a7c439ebdccfd530e2339601ef27.tar.gz"
               ],
               "sha256": "e5470e92a18aa51830db99a4d9c492cc613761d5bdb7131c04bd92b9834380f6",
               "strip_prefix": "rules_python-4b84ad270387a7c439ebdccfd530e2339601ef27"
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
                    "attributes": {
                         "url": "",
                         "urls": [
                              "https://mirror.bazel.build/github.com/bazelbuild/rules_python/archive/4b84ad270387a7c439ebdccfd530e2339601ef27.tar.gz",
                              "https://github.com/bazelbuild/rules_python/archive/4b84ad270387a7c439ebdccfd530e2339601ef27.tar.gz"
                         ],
                         "sha256": "e5470e92a18aa51830db99a4d9c492cc613761d5bdb7131c04bd92b9834380f6",
                         "integrity": "",
                         "netrc": "",
                         "auth_patterns": {},
                         "canonical_id": "",
                         "strip_prefix": "rules_python-4b84ad270387a7c439ebdccfd530e2339601ef27",
                         "add_prefix": "",
                         "type": "",
                         "patches": [],
                         "remote_patches": {},
                         "remote_patch_strip": 0,
                         "patch_tool": "",
                         "patch_args": [
                              "-p0"
                         ],
                         "patch_cmds": [],
                         "patch_cmds_win": [],
                         "build_file_content": "",
                         "workspace_file_content": "",
                         "name": "rules_python"
                    },
                    "output_tree_hash": "09b73abff05a660e6a6c039828e4511368c41a627c8b52f26833b1b57b7ce1a3"
               }
          ]
     },
     {
          "original_rule_class": "local_repository",
          "original_attributes": {
               "name": "parcluster",
               "path": "external/InMemoryClusteringAPI/include"
          },
          "native": "local_repository(name = \"parcluster\", path = \"external/InMemoryClusteringAPI/include\")"
     },
     {
          "original_rule_class": "@@bazel_tools//tools/build_defs/repo:git.bzl%git_repository",
          "definition_information": "Repository com_github_gflags_gflags instantiated at:\n  /home/ubuntu/ParClusterers/WORKSPACE:54:15: in <toplevel>\nRepository rule git_repository defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/git.bzl:189:33: in <toplevel>\n",
          "original_attributes": {
               "name": "com_github_gflags_gflags",
               "remote": "https://github.com/gflags/gflags.git",
               "commit": "f40e43a6288940efadd29c208085db05335a15d8"
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/build_defs/repo:git.bzl%git_repository",
                    "attributes": {
                         "remote": "https://github.com/gflags/gflags.git",
                         "commit": "f40e43a6288940efadd29c208085db05335a15d8",
                         "shallow_since": "",
                         "init_submodules": False,
                         "recursive_init_submodules": False,
                         "verbose": False,
                         "strip_prefix": "",
                         "patches": [],
                         "patch_tool": "",
                         "patch_args": [
                              "-p0"
                         ],
                         "patch_cmds": [],
                         "patch_cmds_win": [],
                         "build_file_content": "",
                         "workspace_file_content": "",
                         "name": "com_github_gflags_gflags"
                    },
                    "output_tree_hash": "2e8b0d7a53c637f6f5f773221c3741df5ec9f4ab337dc45cfe88bd26e602d52f"
               }
          ]
     },
     {
          "original_rule_class": "@@bazel_tools//tools/build_defs/repo:git.bzl%git_repository",
          "definition_information": "Repository com_github_graph_mining instantiated at:\n  /home/ubuntu/ParClusterers/WORKSPACE:24:15: in <toplevel>\nRepository rule git_repository defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/git.bzl:189:33: in <toplevel>\n",
          "original_attributes": {
               "name": "com_github_graph_mining",
               "remote": "https://github.com/google/graph-mining.git",
               "commit": "3fbcb0af2352b459cde2ba104cddd5f07214c584"
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/build_defs/repo:git.bzl%git_repository",
                    "attributes": {
                         "remote": "https://github.com/google/graph-mining.git",
                         "commit": "3fbcb0af2352b459cde2ba104cddd5f07214c584",
                         "shallow_since": "",
                         "init_submodules": False,
                         "recursive_init_submodules": False,
                         "verbose": False,
                         "strip_prefix": "",
                         "patches": [],
                         "patch_tool": "",
                         "patch_args": [
                              "-p0"
                         ],
                         "patch_cmds": [],
                         "patch_cmds_win": [],
                         "build_file_content": "",
                         "workspace_file_content": "",
                         "name": "com_github_graph_mining"
                    },
                    "output_tree_hash": "67a387ffeb5bfcbf57005ed5842fcb990a53725ca63b6e9aa4d403e2f8fce037"
               }
          ]
     },
     {
          "original_rule_class": "@@bazel_tools//tools/build_defs/repo:git.bzl%git_repository",
          "definition_information": "Repository parlaylib instantiated at:\n  /home/ubuntu/ParClusterers/WORKSPACE:17:15: in <toplevel>\nRepository rule git_repository defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/git.bzl:189:33: in <toplevel>\n",
          "original_attributes": {
               "name": "parlaylib",
               "remote": "https://github.com/ParAlg/parlaylib.git",
               "commit": "6b4a4cdbfeb3c481608a42db0230eb6ebb87bf8d",
               "strip_prefix": "include/"
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/build_defs/repo:git.bzl%git_repository",
                    "attributes": {
                         "remote": "https://github.com/ParAlg/parlaylib.git",
                         "commit": "6b4a4cdbfeb3c481608a42db0230eb6ebb87bf8d",
                         "shallow_since": "",
                         "init_submodules": False,
                         "recursive_init_submodules": False,
                         "verbose": False,
                         "strip_prefix": "include/",
                         "patches": [],
                         "patch_tool": "",
                         "patch_args": [
                              "-p0"
                         ],
                         "patch_cmds": [],
                         "patch_cmds_win": [],
                         "build_file_content": "",
                         "workspace_file_content": "",
                         "name": "parlaylib"
                    },
                    "output_tree_hash": "a88ff6b81234d2781954d0ac2c5696e5cf7a137904369c7407fd840221637045"
               }
          ]
     },
     {
          "original_rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
          "definition_information": "Repository com_github_protocolbuffers_protobuf instantiated at:\n  /home/ubuntu/ParClusterers/WORKSPACE:77:25: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_proto/proto/repositories.bzl:29:14: in rules_proto_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\nRepository rule http_archive defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/http.bzl:381:31: in <toplevel>\n",
          "original_attributes": {
               "name": "com_github_protocolbuffers_protobuf",
               "generator_name": "com_github_protocolbuffers_protobuf",
               "generator_function": "rules_proto_dependencies",
               "generator_location": None,
               "urls": [
                    "https://mirror.bazel.build/github.com/protocolbuffers/protobuf/archive/v3.17.0.tar.gz",
                    "https://github.com/protocolbuffers/protobuf/archive/v3.17.0.tar.gz"
               ],
               "sha256": "eaba1dd133ac5167e8b08bc3268b2d33c6e9f2dcb14ec0f97f3d3eed9b395863",
               "strip_prefix": "protobuf-3.17.0"
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
                    "attributes": {
                         "url": "",
                         "urls": [
                              "https://mirror.bazel.build/github.com/protocolbuffers/protobuf/archive/v3.17.0.tar.gz",
                              "https://github.com/protocolbuffers/protobuf/archive/v3.17.0.tar.gz"
                         ],
                         "sha256": "eaba1dd133ac5167e8b08bc3268b2d33c6e9f2dcb14ec0f97f3d3eed9b395863",
                         "integrity": "",
                         "netrc": "",
                         "auth_patterns": {},
                         "canonical_id": "",
                         "strip_prefix": "protobuf-3.17.0",
                         "add_prefix": "",
                         "type": "",
                         "patches": [],
                         "remote_patches": {},
                         "remote_patch_strip": 0,
                         "patch_tool": "",
                         "patch_args": [
                              "-p0"
                         ],
                         "patch_cmds": [],
                         "patch_cmds_win": [],
                         "build_file_content": "",
                         "workspace_file_content": "",
                         "name": "com_github_protocolbuffers_protobuf"
                    },
                    "output_tree_hash": "4986cf0a34d21a343115a7d7734548cc1b0d4833a78a9c8a36f06cfff9a823c8"
               }
          ]
     },
     {
          "original_rule_class": "@@bazel_tools//tools/cpp:cc_configure.bzl%cc_autoconf",
          "definition_information": "Repository local_config_cc instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:168:13: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/cpp/cc_configure.bzl:149:16: in cc_configure\nRepository rule cc_autoconf defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/cpp/cc_configure.bzl:109:30: in <toplevel>\n",
          "original_attributes": {
               "name": "local_config_cc",
               "generator_name": "local_config_cc",
               "generator_function": "cc_configure",
               "generator_location": None
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/cpp:cc_configure.bzl%cc_autoconf",
                    "attributes": {
                         "name": "local_config_cc",
                         "generator_name": "local_config_cc",
                         "generator_function": "cc_configure",
                         "generator_location": None
                    },
                    "output_tree_hash": "7f2858a5db786cbed74c8127b997cf738049b6c8e2cfa1d6a78170319efd9dfc"
               }
          ]
     },
     {
          "original_rule_class": "@@bazel_tools//tools/build_defs/repo:git.bzl%git_repository",
          "definition_information": "Repository gbbs instantiated at:\n  /home/ubuntu/ParClusterers/WORKSPACE:11:15: in <toplevel>\nRepository rule git_repository defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/git.bzl:189:33: in <toplevel>\n",
          "original_attributes": {
               "name": "gbbs",
               "remote": "https://github.com/ParAlg/gbbs.git",
               "branch": "master"
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/build_defs/repo:git.bzl%git_repository",
                    "attributes": {
                         "remote": "https://github.com/ParAlg/gbbs.git",
                         "commit": "8b0f34037e699d394a788ae14018d564ce226662",
                         "shallow_since": "",
                         "init_submodules": False,
                         "recursive_init_submodules": False,
                         "verbose": False,
                         "strip_prefix": "",
                         "patches": [],
                         "patch_tool": "",
                         "patch_args": [
                              "-p0"
                         ],
                         "patch_cmds": [],
                         "patch_cmds_win": [],
                         "build_file_content": "",
                         "workspace_file_content": "",
                         "name": "gbbs"
                    },
                    "output_tree_hash": "a86b0ddde64517feda04548224b73bf1a7c1744c49523b9255d63e5d792b6c34"
               }
          ]
     },
     {
          "original_rule_class": "@@bazel_tools//tools/osx:xcode_configure.bzl%xcode_autoconf",
          "definition_information": "Repository local_config_xcode instantiated at:\n  /DEFAULT.WORKSPACE.SUFFIX:171:16: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/osx/xcode_configure.bzl:312:19: in xcode_configure\nRepository rule xcode_autoconf defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/osx/xcode_configure.bzl:297:33: in <toplevel>\n",
          "original_attributes": {
               "name": "local_config_xcode",
               "generator_name": "local_config_xcode",
               "generator_function": "xcode_configure",
               "generator_location": None,
               "xcode_locator": "@bazel_tools//tools/osx:xcode_locator.m"
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/osx:xcode_configure.bzl%xcode_autoconf",
                    "attributes": {
                         "name": "local_config_xcode",
                         "generator_name": "local_config_xcode",
                         "generator_function": "xcode_configure",
                         "generator_location": None,
                         "xcode_locator": "@bazel_tools//tools/osx:xcode_locator.m"
                    },
                    "output_tree_hash": "ec026961157bb71cf68d1b568815ad68147ed16f318161bc0da40f6f3d7d79fd"
               }
          ]
     },
     {
          "original_rule_class": "@@rules_proto//proto/private:dependencies.bzl%protobuf_workspace",
          "definition_information": "Repository com_google_protobuf instantiated at:\n  /home/ubuntu/ParClusterers/WORKSPACE:77:25: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_proto/proto/repositories.bzl:32:23: in rules_proto_dependencies\nRepository rule protobuf_workspace defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_proto/proto/private/dependencies.bzl:192:37: in <toplevel>\n",
          "original_attributes": {
               "name": "com_google_protobuf",
               "generator_name": "com_google_protobuf",
               "generator_function": "rules_proto_dependencies",
               "generator_location": None
          },
          "repositories": [
               {
                    "rule_class": "@@rules_proto//proto/private:dependencies.bzl%protobuf_workspace",
                    "attributes": {
                         "name": "com_google_protobuf",
                         "generator_name": "com_google_protobuf",
                         "generator_function": "rules_proto_dependencies",
                         "generator_location": None
                    },
                    "output_tree_hash": "0cc334f0a0cf881ae2a69cd02481c7ab6d3b02d86e0af7da269d0246dc519ab1"
               }
          ]
     },
     {
          "original_rule_class": "@@bazel_tools//tools/build_defs/repo:git.bzl%git_repository",
          "definition_information": "Repository com_google_absl instantiated at:\n  /home/ubuntu/ParClusterers/WORKSPACE:148:15: in <toplevel>\nRepository rule git_repository defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/git.bzl:189:33: in <toplevel>\n",
          "original_attributes": {
               "name": "com_google_absl",
               "remote": "https://github.com/abseil/abseil-cpp.git",
               "tag": "20230125.2"
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/build_defs/repo:git.bzl%git_repository",
                    "attributes": {
                         "remote": "https://github.com/abseil/abseil-cpp.git",
                         "commit": "b971ac5250ea8de900eae9f95e06548d14cd95fe",
                         "shallow_since": "",
                         "init_submodules": False,
                         "recursive_init_submodules": False,
                         "verbose": False,
                         "strip_prefix": "",
                         "patches": [],
                         "patch_tool": "",
                         "patch_args": [
                              "-p0"
                         ],
                         "patch_cmds": [],
                         "patch_cmds_win": [],
                         "build_file_content": "",
                         "workspace_file_content": "",
                         "name": "com_google_absl"
                    },
                    "output_tree_hash": "5aae0c36b0d7a14c104b1945762736c458cd6c590c03dc0851f315cbcecf6778"
               }
          ]
     },
     {
          "original_rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
          "definition_information": "Repository com_google_protobuf_protoc_linux_x86_64 instantiated at:\n  /home/ubuntu/ParClusterers/WORKSPACE:77:25: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_proto/proto/repositories.bzl:29:14: in rules_proto_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\nRepository rule http_archive defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/http.bzl:381:31: in <toplevel>\n",
          "original_attributes": {
               "name": "com_google_protobuf_protoc_linux_x86_64",
               "generator_name": "com_google_protobuf_protoc_linux_x86_64",
               "generator_function": "rules_proto_dependencies",
               "generator_location": None,
               "urls": [
                    "https://mirror.bazel.build/github.com/protocolbuffers/protobuf/releases/download/v3.17.0/protoc-3.17.0-linux-x86_64.zip",
                    "https://github.com/protocolbuffers/protobuf/releases/download/v3.17.0/protoc-3.17.0-linux-x86_64.zip"
               ],
               "sha256": "aad5cfd2daf9d49f5ec9b14c4e7e7af0392324706c0e5bb3e44ad5e70a7add5e",
               "build_file": "@@rules_proto//proto/private:BUILD.protoc"
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
                    "attributes": {
                         "url": "",
                         "urls": [
                              "https://mirror.bazel.build/github.com/protocolbuffers/protobuf/releases/download/v3.17.0/protoc-3.17.0-linux-x86_64.zip",
                              "https://github.com/protocolbuffers/protobuf/releases/download/v3.17.0/protoc-3.17.0-linux-x86_64.zip"
                         ],
                         "sha256": "aad5cfd2daf9d49f5ec9b14c4e7e7af0392324706c0e5bb3e44ad5e70a7add5e",
                         "integrity": "",
                         "netrc": "",
                         "auth_patterns": {},
                         "canonical_id": "",
                         "strip_prefix": "",
                         "add_prefix": "",
                         "type": "",
                         "patches": [],
                         "remote_patches": {},
                         "remote_patch_strip": 0,
                         "patch_tool": "",
                         "patch_args": [
                              "-p0"
                         ],
                         "patch_cmds": [],
                         "patch_cmds_win": [],
                         "build_file": "@@rules_proto//proto/private:BUILD.protoc",
                         "build_file_content": "",
                         "workspace_file_content": "",
                         "name": "com_google_protobuf_protoc_linux_x86_64"
                    },
                    "output_tree_hash": "568afca8c3ddba0cf5f6f539afb35355b15f2d60458a1a87bf2a71df7b058cd2"
               }
          ]
     },
     {
          "original_rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
          "definition_information": "Repository bazel_skylib instantiated at:\n  /home/ubuntu/ParClusterers/WORKSPACE:77:25: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_proto/proto/repositories.bzl:29:14: in rules_proto_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\nRepository rule http_archive defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/http.bzl:381:31: in <toplevel>\n",
          "original_attributes": {
               "name": "bazel_skylib",
               "generator_name": "bazel_skylib",
               "generator_function": "rules_proto_dependencies",
               "generator_location": None,
               "urls": [
                    "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.0.2/bazel-skylib-1.0.2.tar.gz",
                    "https://github.com/bazelbuild/bazel-skylib/releases/download/1.0.2/bazel-skylib-1.0.2.tar.gz"
               ],
               "sha256": "97e70364e9249702246c0e9444bccdc4b847bed1eb03c5a3ece4f83dfe6abc44"
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
                    "attributes": {
                         "url": "",
                         "urls": [
                              "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.0.2/bazel-skylib-1.0.2.tar.gz",
                              "https://github.com/bazelbuild/bazel-skylib/releases/download/1.0.2/bazel-skylib-1.0.2.tar.gz"
                         ],
                         "sha256": "97e70364e9249702246c0e9444bccdc4b847bed1eb03c5a3ece4f83dfe6abc44",
                         "integrity": "",
                         "netrc": "",
                         "auth_patterns": {},
                         "canonical_id": "",
                         "strip_prefix": "",
                         "add_prefix": "",
                         "type": "",
                         "patches": [],
                         "remote_patches": {},
                         "remote_patch_strip": 0,
                         "patch_tool": "",
                         "patch_args": [
                              "-p0"
                         ],
                         "patch_cmds": [],
                         "patch_cmds_win": [],
                         "build_file_content": "",
                         "workspace_file_content": "",
                         "name": "bazel_skylib"
                    },
                    "output_tree_hash": "9d894c3050dd6f85c271d712b7a79dbcc4057c553a9183d7dc791ff208326f9f"
               }
          ]
     },
     {
          "original_rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
          "definition_information": "Repository rules_java instantiated at:\n  /home/ubuntu/ParClusterers/WORKSPACE:77:25: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_proto/proto/repositories.bzl:29:14: in rules_proto_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\nRepository rule http_archive defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/http.bzl:381:31: in <toplevel>\n",
          "original_attributes": {
               "name": "rules_java",
               "generator_name": "rules_java",
               "generator_function": "rules_proto_dependencies",
               "generator_location": None,
               "urls": [
                    "https://mirror.bazel.build/github.com/bazelbuild/rules_java/archive/981f06c3d2bd10225e85209904090eb7b5fb26bd.tar.gz",
                    "https://github.com/bazelbuild/rules_java/archive/981f06c3d2bd10225e85209904090eb7b5fb26bd.tar.gz"
               ],
               "sha256": "f5a3e477e579231fca27bf202bb0e8fbe4fc6339d63b38ccb87c2760b533d1c3",
               "strip_prefix": "rules_java-981f06c3d2bd10225e85209904090eb7b5fb26bd"
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
                    "attributes": {
                         "url": "",
                         "urls": [
                              "https://mirror.bazel.build/github.com/bazelbuild/rules_java/archive/981f06c3d2bd10225e85209904090eb7b5fb26bd.tar.gz",
                              "https://github.com/bazelbuild/rules_java/archive/981f06c3d2bd10225e85209904090eb7b5fb26bd.tar.gz"
                         ],
                         "sha256": "f5a3e477e579231fca27bf202bb0e8fbe4fc6339d63b38ccb87c2760b533d1c3",
                         "integrity": "",
                         "netrc": "",
                         "auth_patterns": {},
                         "canonical_id": "",
                         "strip_prefix": "rules_java-981f06c3d2bd10225e85209904090eb7b5fb26bd",
                         "add_prefix": "",
                         "type": "",
                         "patches": [],
                         "remote_patches": {},
                         "remote_patch_strip": 0,
                         "patch_tool": "",
                         "patch_args": [
                              "-p0"
                         ],
                         "patch_cmds": [],
                         "patch_cmds_win": [],
                         "build_file_content": "",
                         "workspace_file_content": "",
                         "name": "rules_java"
                    },
                    "output_tree_hash": "b7f599f535ae696f80ee7382299b50cfa47156b99548edc7a1895ef71a36c78b"
               }
          ]
     },
     {
          "original_rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
          "definition_information": "Repository zlib instantiated at:\n  /home/ubuntu/ParClusterers/WORKSPACE:77:25: in <toplevel>\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/rules_proto/proto/repositories.bzl:29:14: in rules_proto_dependencies\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/utils.bzl:240:18: in maybe\nRepository rule http_archive defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/http.bzl:381:31: in <toplevel>\n",
          "original_attributes": {
               "name": "zlib",
               "generator_name": "zlib",
               "generator_function": "rules_proto_dependencies",
               "generator_location": None,
               "urls": [
                    "https://mirror.bazel.build/zlib.net/zlib-1.2.11.tar.gz",
                    "https://zlib.net/zlib-1.2.11.tar.gz"
               ],
               "sha256": "c3e5e9fdd5004dcb542feda5ee4f0ff0744628baf8ed2dd5d66f8ca1197cb1a1",
               "strip_prefix": "zlib-1.2.11",
               "build_file": "@@com_github_protocolbuffers_protobuf//:third_party/zlib.BUILD"
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
                    "attributes": {
                         "url": "",
                         "urls": [
                              "https://mirror.bazel.build/zlib.net/zlib-1.2.11.tar.gz",
                              "https://zlib.net/zlib-1.2.11.tar.gz"
                         ],
                         "sha256": "c3e5e9fdd5004dcb542feda5ee4f0ff0744628baf8ed2dd5d66f8ca1197cb1a1",
                         "integrity": "",
                         "netrc": "",
                         "auth_patterns": {},
                         "canonical_id": "",
                         "strip_prefix": "zlib-1.2.11",
                         "add_prefix": "",
                         "type": "",
                         "patches": [],
                         "remote_patches": {},
                         "remote_patch_strip": 0,
                         "patch_tool": "",
                         "patch_args": [
                              "-p0"
                         ],
                         "patch_cmds": [],
                         "patch_cmds_win": [],
                         "build_file": "@@com_github_protocolbuffers_protobuf//:third_party/zlib.BUILD",
                         "build_file_content": "",
                         "workspace_file_content": "",
                         "name": "zlib"
                    },
                    "output_tree_hash": "f06673044ec9e62145abb53d156d42480294ea63ac67ef865d30ae4509a95e74"
               }
          ]
     },
     {
          "original_rule_class": "@@bazel_tools//tools/build_defs/repo:git.bzl%git_repository",
          "definition_information": "Repository com_github_gbbs instantiated at:\n  /home/ubuntu/ParClusterers/WORKSPACE:42:15: in <toplevel>\nRepository rule git_repository defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/git.bzl:189:33: in <toplevel>\n",
          "original_attributes": {
               "name": "com_github_gbbs",
               "remote": "https://github.com/ParAlg/gbbs.git",
               "branch": "master"
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/build_defs/repo:git.bzl%git_repository",
                    "attributes": {
                         "remote": "https://github.com/ParAlg/gbbs.git",
                         "commit": "8b0f34037e699d394a788ae14018d564ce226662",
                         "shallow_since": "",
                         "init_submodules": False,
                         "recursive_init_submodules": False,
                         "verbose": False,
                         "strip_prefix": "",
                         "patches": [],
                         "patch_tool": "",
                         "patch_args": [
                              "-p0"
                         ],
                         "patch_cmds": [],
                         "patch_cmds_win": [],
                         "build_file_content": "",
                         "workspace_file_content": "",
                         "name": "com_github_gbbs"
                    },
                    "output_tree_hash": "08bc94bc88e2d1e73e5cea460236b0591b8af4e0b8e2aede39b30fcba7580be4"
               }
          ]
     },
     {
          "original_rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
          "definition_information": "Repository com_google_googletest instantiated at:\n  /home/ubuntu/ParClusterers/WORKSPACE:167:13: in <toplevel>\nRepository rule http_archive defined at:\n  /home/ubuntu/.cache/bazel/_bazel_ubuntu/1cb952b413f6b9345fe8bb2c929ef153/external/bazel_tools/tools/build_defs/repo/http.bzl:381:31: in <toplevel>\n",
          "original_attributes": {
               "name": "com_google_googletest",
               "urls": [
                    "https://github.com/google/googletest/archive/5ab508a01f9eb089207ee87fd547d290da39d015.zip"
               ],
               "strip_prefix": "googletest-5ab508a01f9eb089207ee87fd547d290da39d015"
          },
          "repositories": [
               {
                    "rule_class": "@@bazel_tools//tools/build_defs/repo:http.bzl%http_archive",
                    "attributes": {
                         "url": "",
                         "urls": [
                              "https://github.com/google/googletest/archive/5ab508a01f9eb089207ee87fd547d290da39d015.zip"
                         ],
                         "sha256": "",
                         "integrity": "sha256-dV+aObxyBfWgxCjpIN2tCSwzyKG0aZfe8/HUqCre1uE=",
                         "netrc": "",
                         "auth_patterns": {},
                         "canonical_id": "",
                         "strip_prefix": "googletest-5ab508a01f9eb089207ee87fd547d290da39d015",
                         "add_prefix": "",
                         "type": "",
                         "patches": [],
                         "remote_patches": {},
                         "remote_patch_strip": 0,
                         "patch_tool": "",
                         "patch_args": [
                              "-p0"
                         ],
                         "patch_cmds": [],
                         "patch_cmds_win": [],
                         "build_file_content": "",
                         "workspace_file_content": "",
                         "name": "com_google_googletest"
                    },
                    "output_tree_hash": "d0929039710c8d66da52ca61e1346a93de234ccd84271c8107a1e8ddb83e4630"
               }
          ]
     }
]