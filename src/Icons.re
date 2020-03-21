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

type simpleIcon = {
  svg: string,
  title: string,
  hex: string,
  path: string,
};

module MakeSimpleIcon = (IconData: {let icon: simpleIcon;}) => {
  [@react.component]
  let make =
      (
        ~height="24",
        ~width="24",
        ~className="",
        ~style=ReactDOMRe.Style.make(),
        ~ariaLabel=IconData.icon.title ++ " Icon",
        ~ariaHidden=false,
      ) =>
    <svg
      role="img"
      viewBox="0 0 24 24"
      height
      width
      className
      style=ReactDOMRe.Style.(
        make(~fill="#" ++ IconData.icon.hex, ())->combine(style)
      )
      ariaLabel
      ariaHidden>
      <path d={IconData.icon.path} />
    </svg>;
  React.setDisplayName(make, IconData.icon.title);
};

module Reason =
  MakeSimpleIcon({
    [@bs.module] external icon: simpleIcon = "simple-icons/icons/reason";
  });

module React =
  MakeSimpleIcon({
    [@bs.module] external icon: simpleIcon = "simple-icons/icons/react";
  });

module D3 =
  MakeSimpleIcon({
    [@bs.module] external icon: simpleIcon = "simple-icons/icons/d3-dot-js";
  });

module GitHub =
  MakeSimpleIcon({
    [@bs.module] external icon: simpleIcon = "simple-icons/icons/github";
  });

module NodeJs =
  MakeSimpleIcon({
    [@bs.module] external icon: simpleIcon = "simple-icons/icons/node-dot-js";
  });

module SimpleIcons =
  MakeSimpleIcon({
    [@bs.module] external icon: simpleIcon = "simple-icons/icons/simpleicons";
  });
