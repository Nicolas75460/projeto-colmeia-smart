"use client";
import React, { useState } from "react";
import { Button } from "../../ui/button";
import { useNewDeviceForm } from "./useNewDeviceForm";
import { DeviceDialog } from "../shared/DeviceDialog";
import { BaseDeviceForm } from "../shared/BaseDeviceForm";
import { FormProvider } from "react-hook-form";
import { EditableDeviceFooter } from "../shared/EditableDeviceFooter";

export default function NewDeviceDialog() {
  const [open, setOpen] = useState(false);

  const form = useNewDeviceForm();

  const onSubmit = form.handleSubmit(async (data) => {
      await new Promise(resolve => setTimeout(resolve, 2000));
      form.reset();
      setOpen(false);

  });

  const handleDialogChange = (isOpen: boolean) => {
    setOpen(isOpen);
    if (!isOpen) {
      form.reset();
    }
  };

  return (
    <FormProvider {...form}>
    <DeviceDialog
      trigger={<Button>Adicionar dispositivo</Button>}
      title="Novo dispositivo"
      description="Primeiro, escolha um nome para sua colmeia, e depois, selecione uma predefinição"
      onSubmit={onSubmit}
      open={open}
      onOpenChange={handleDialogChange}
    >
      <BaseDeviceForm/>
      <EditableDeviceFooter onCancel={() => setOpen(false)} />
    </DeviceDialog>
  </FormProvider>
  );
}