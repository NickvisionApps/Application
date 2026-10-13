import {invoke} from "@tauri-apps/api/core";
import {ReactNode, useEffect, useMemo, useState} from "react";

import {
  ProductInfo,
  ProductInfoProviderContext,
  ProductInfoProviderState,
} from "@/lib/contexts/product-info-context.ts";

interface ProductInfoProviderProps {
  children: ReactNode;
}

const DefaultProductInfo: ProductInfo = {
  id: "",
  name: "",
  short_name: "",
  repo_owner: "",
  repo_name: "",
  description: "",
  version: "",
};

export function ProductInfoProvider({
  children,
  ...props
}: ProductInfoProviderProps) {
  const [productInfo, setProductInfo] =
    useState<ProductInfo>(DefaultProductInfo);

  useEffect(() => {
    async function startup() {
      setProductInfo(await invoke("get_product_information"));
    }

    void startup();
  }, []);

  return (
    <ProductInfoProviderContext.Provider
      {...props}
      value={useMemo<ProductInfoProviderState>(
        () => ({
          productInfo,
        }),
        [productInfo],
      )}
    >
      {children}
    </ProductInfoProviderContext.Provider>
  );
}
