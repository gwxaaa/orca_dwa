
(cl:in-package :asdf)

(defsystem "orca_dwa-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
)
  :components ((:file "_package")
    (:file "test" :depends-on ("_package_test"))
    (:file "_package_test" :depends-on ("_package"))
  ))