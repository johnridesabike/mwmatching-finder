module ChevronLeft = {
  [@bs.module "react-feather/dist/icons/chevron-left"] [@react.component]
  external make:
    (~className: string=?, ~style: ReactDOMRe.Style.t=?) => React.element =
    "default";
};

module ChevronDown = {
  [@bs.module "react-feather/dist/icons/chevron-down"] [@react.component]
  external make:
    (~className: string=?, ~style: ReactDOMRe.Style.t=?) => React.element =
    "default";
};

module type SimpleIcon = {
  let svg: string;
  let title: string;
  let hex: string;
  let path: string;
};

module MakeSimpleIcon = (Data: SimpleIcon) => {
  [@react.component]
  let make =
      (
        ~height="24",
        ~width="24",
        ~className="",
        ~style=ReactDOMRe.Style.make(),
        ~fill="#" ++ Data.hex,
        ~ariaLabel=Data.title ++ " Icon",
        ~ariaHidden=false,
      ) =>
    <svg
      role="img"
      viewBox="0 0 24 24"
      height
      width
      className
      style
      ariaLabel
      ariaHidden>
      <path d=Data.path fill />
    </svg>;
  React.setDisplayName(make, Data.title);
};

[@bs.module]
external icon: (module SimpleIcon) = "simple-icons/icons/reason";
module Reason = MakeSimpleIcon((val icon));

[@bs.module] external icon: (module SimpleIcon) = "simple-icons/icons/d3-dot-js";
module D3 = MakeSimpleIcon((val icon));

[@bs.module]
external icon: (module SimpleIcon) = "simple-icons/icons/github";
module GitHub = MakeSimpleIcon((val icon));

[@bs.module]
external icon: (module SimpleIcon) = "simple-icons/icons/node-dot-js";
module NodeJs = MakeSimpleIcon((val icon));

[@bs.module]
external icon: (module SimpleIcon) = "simple-icons/icons/simpleicons";
module SimpleIcons = MakeSimpleIcon((val icon));

[@bs.module] external icon: (module SimpleIcon) = "simple-icons/icons/react";
module React = MakeSimpleIcon((val icon));
